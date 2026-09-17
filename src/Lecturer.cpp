#include "Lecturer.h"
#include "Course.h"
#include "Student.h"
#include "AttendanceSession.h"
#include "AttendanceRegister.h"
#include "AttendanceCapture.h"
#include "CardTapCapture.h"
#include "StudentCard.h"
#include "FileReplayCapture.h"
#include "LMSException.h"
#include "SystemManager.h"
#include <iostream>
#include <algorithm>
#include <memory>

namespace domain {

// Integration state for the lecturer menu. Kept in this .cpp so the Lecturer
// class declaration stays aligned with the supplied UML.
namespace {
std::unique_ptr<attendance::AttendanceSession> activeSession;
attendance::AttendanceRegister attendanceRegister;
std::unique_ptr<attendance::AttendanceCapture> captureMethod;
int nextSessionId = 1;
}

Lecturer::Lecturer(std::string personId, std::string name, std::string email, std::string password,
                    std::string department, std::string staffId)
    : Person(std::move(personId), std::move(name), std::move(email), std::move(password)),
      department_(std::move(department)), staffId_(std::move(staffId)) {}

Dashboard Lecturer::getDashboard() const {
    return Dashboard{};
}

void Lecturer::openAttendance() {
    if (courses_.empty()) {
        std::cout << "No courses assigned to this lecturer.\n";
        return;
    }

    std::cout << "\nCourses:\n";
    for (auto* course : courses_) {
        if (course) std::cout << "  " << course->getCode() << " - " << course->getTitle() << "\n";
    }

    std::cout << "Enter course code: ";
    std::string code;
    if (!std::getline(std::cin, code)) return;

    Course* selected = nullptr;
    for (auto* course : courses_) {
        if (course && course->getCode() == code) {
            selected = course;
            break;
        }
    }

    if (!selected) {
        std::cout << "Course not found.\n";
        return;
    }

    if (activeSession && activeSession->isOpen()) activeSession->close();

    // Do not reuse a persisted session ID after an application restart.
    // The live counter starts at 1 each process, so advance it past any
    // session IDs already stored by the attendance repository.
    for (const auto& saved : repo::SystemManager::getInstance().getAttendanceRepository().getItems()) {
        if (saved.sessionId >= nextSessionId) nextSessionId = saved.sessionId + 1;
    }

    activeSession = std::make_unique<attendance::AttendanceSession>(nextSessionId++, *selected, 5);
    activeSession->open();
    std::cout << "Attendance session " << activeSession->getSessionId()
              << " opened for " << selected->getCode() << ".\n";
}

void Lecturer::markAttendance() {
    if (!activeSession || !activeSession->isOpen()) {
        std::cout << "No open attendance session.\n";
        return;
    }

    Course& course = activeSession->getCourse();
    const auto& students = course.getEnrolledStudents();
    if (students.empty()) {
        std::cout << "No students are enrolled in this course.\n";
        return;
    }

    // If a capture strategy was selected, use it to obtain card UIDs.
    // For this console simulation each student's card UID is their unique
    // student ID. StudentCard still performs the UID -> Student mapping.
    if (captureMethod) {
        try {
            captureMethod->beginSession();
        } catch (const std::exception& ex) {
            std::cout << "Capture error: " << ex.what() << "\n";
            return;
        }

        while (activeSession->isOpen()) {
            std::optional<std::string> capturedUID;
            try {
                capturedUID = captureMethod->captureNext();
            } catch (const std::exception& ex) {
                std::cout << "Capture error: " << ex.what() << "\n";
                continue;
            }
            if (!capturedUID) break;

            Student* selected = nullptr;
            for (auto* student : students) {
                if (!student) continue;
                attendance::StudentCard card(student->getStudentId(), student);
                if (card.getUID() == *capturedUID) {
                    selected = card.getOwner();
                    break;
                }
            }

            if (!selected) {
                std::cout << "Unknown card UID: " << *capturedUID << "\n";
                continue;
            }

            try {
                attendanceRegister.markAttendance(*selected, *activeSession, "CardTap");
                // Integration: keep a persistable copy so Student::viewAttendance()
                // can read the same attendance data through SystemManager.
                repo::SystemManager::getInstance().getAttendanceRepository()
                    .addFromDomainRecord(attendanceRegister.getRecords().back());
                std::cout << "Attendance marked for " << selected->getStudentId() << ".\n";
            } catch (const std::exception& ex) {
                std::cout << "Attendance error: " << ex.what() << "\n";
            }
        }

        captureMethod->endSession();
        return;
    }

    // Simple manual fallback when the lecturer has not selected a capture method.
    std::cout << "Enter student ID: ";
    std::string studentId;
    if (!std::getline(std::cin, studentId)) return;

    Student* selected = nullptr;
    for (auto* student : students) {
        if (student && student->getStudentId() == studentId) {
            selected = student;
            break;
        }
    }

    if (!selected) {
        std::cout << "Student is not enrolled in " << course.getCode() << ".\n";
        return;
    }

    try {
        attendanceRegister.markAttendance(*selected, *activeSession, "Lecturer");
        repo::SystemManager::getInstance().getAttendanceRepository()
            .addFromDomainRecord(attendanceRegister.getRecords().back());
        std::cout << "Attendance marked for " << selected->getStudentId() << ".\n";
    } catch (const std::exception& ex) {
        std::cout << "Attendance error: " << ex.what() << "\n";
    }
}

void Lecturer::viewCourses() const {
    std::cout << "[Lecturer " << staffId_ << "] teaches " << courses_.size() << " course(s)\n";
    for (auto* course : courses_) {
        if (course) std::cout << "  " << course->getCode() << " - " << course->getTitle() << "\n";
    }
}

void Lecturer::viewEnrollmentList() const {
    if (courses_.empty()) {
        std::cout << "No courses assigned to this lecturer.\n";
        return;
    }
    std::cout << "Enter course code: ";
    std::string code;
    if (!std::getline(std::cin, code)) return;
    Course* selected = nullptr;
    for (auto* course : courses_) if (course && course->getCode() == code) { selected = course; break; }
    if (!selected) {
        std::cout << "You are not assigned to that course.\n";
        return;
    }
    std::cout << "\n--- Enrollment List: " << selected->getCode() << " ---\n";
    const auto& students = selected->getEnrolledStudents();
    if (students.empty()) { std::cout << "No enrolled students.\n"; return; }
    for (auto* student : students) if (student)
        std::cout << "  " << student->getStudentId() << " | " << student->getName() << "\n";
    std::cout << "Total enrolled: " << students.size() << "\n";
}

void Lecturer::selectCaptureMethod() {
    std::cout << "Select capture method:\n"
              << "1) Card Tap\n"
              << "2) File Replay\n"
              << "Choice: ";
    std::string choice;
    if (!std::getline(std::cin, choice)) return;

    if (choice == "1") {
        captureMethod = std::make_unique<attendance::CardTapCapture>();
        std::cout << "Card Tap capture selected.\n";
    } else if (choice == "2") {
        std::cout << "Enter replay file path: ";
        std::string path;
        if (!std::getline(std::cin, path)) return;
        captureMethod = std::make_unique<attendance::FileReplayCapture>(path);
        std::cout << "File Replay capture selected.\n";
    } else {
        std::cout << "Invalid capture method.\n";
    }
}


void Lecturer::closeAttendance() {
    if (!activeSession || !activeSession->isOpen()) {
        std::cout << "No open attendance session.\n";
        return;
    }
    const int id = activeSession->getSessionId();
    activeSession->close();
    std::cout << "Attendance session " << id << " closed.\n";
}

void Lecturer::viewAttendanceReport() const {
    const auto& records = repo::SystemManager::getInstance()
                              .getAttendanceRepository().getItems();
    if (records.empty()) {
        std::cout << "No attendance records found.\n";
        return;
    }

    std::cout << "\n--- Attendance Report ---\n";
    for (const auto& record : records) {
        std::cout << record.courseCode
                  << " | Session " << record.sessionId
                  << " | " << record.studentId
                  << " | " << record.status
                  << " | " << record.captureMethod << "\n";
    }
    std::cout << "Total stored records: " << records.size() << "\n";
}

void Lecturer::correctAttendance() {
    if (!activeSession) {
        std::cout << "No attendance session available for correction.\n";
        return;
    }

    Course& course = activeSession->getCourse();
    std::cout << "Enter student ID to correct: ";
    std::string studentId;
    if (!std::getline(std::cin, studentId)) return;

    Student* selected = nullptr;
    for (auto* student : course.getEnrolledStudents()) {
        if (student && student->getStudentId() == studentId) {
            selected = student;
            break;
        }
    }
    if (!selected) {
        std::cout << "Student is not enrolled in " << course.getCode() << ".\n";
        return;
    }

    std::cout << "Corrected status (Present/Absent): ";
    std::string status;
    if (!std::getline(std::cin, status)) return;
    if (status != "Present" && status != "Absent") {
        std::cout << "Invalid status. Use Present or Absent.\n";
        return;
    }

    std::cout << "Reason: ";
    std::string reason;
    if (!std::getline(std::cin, reason)) return;

    try {
        attendanceRegister.correctAttendance(*selected, *activeSession, status, reason);
        repo::SystemManager::getInstance().getAttendanceRepository()
            .addFromDomainRecord(attendanceRegister.getRecords().back());
        std::cout << "Attendance correction appended for " << studentId << ".\n";
    } catch (const std::exception& ex) {
        std::cout << "Attendance correction error: " << ex.what() << "\n";
    }
}

const std::string& Lecturer::getDepartment() const noexcept { return department_; }
const std::string& Lecturer::getStaffId() const noexcept { return staffId_; }

void Lecturer::assignCourse(Course* course) {
    if (course != nullptr) {
        courses_.push_back(course);
        course->setLecturer(this);
    }
}

void Lecturer::removeCourse(const Course* course) {
    courses_.erase(std::remove(courses_.begin(), courses_.end(), course), courses_.end());
}

const std::vector<Course*>& Lecturer::getCourses() const noexcept { return courses_; }

} // namespace domain
