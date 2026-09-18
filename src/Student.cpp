#include "Student.h"
#include "Course.h"
#include "Enrollment.h"
#include "NotEnrolledException.h"
#include <algorithm>
#include <iostream>
#include <map>
#include "SystemManager.h"

namespace domain {

Student::Student(std::string personId, std::string name, std::string email, std::string password,
                  std::string studentId)
    : Person(std::move(personId), std::move(name), std::move(email), std::move(password)),
      studentId_(std::move(studentId)) {}

Dashboard Student::getDashboard() const {
    return Dashboard{};
}

void Student::registerCourse(Course& course) {
    auto enrollment = std::make_shared<Enrollment>(*this, course);
    enrollment->enroll(); // may throw; if it does, we simply don't record it below
    enrollments_.push_back(enrollment);
}

void Student::dropCourse(const std::string& courseCode) {
    auto it = std::find_if(enrollments_.begin(), enrollments_.end(),
        [&](const std::shared_ptr<Enrollment>& e) {
            return e->isActive() && e->getCourse()->getCode() == courseCode;
        });

    if (it == enrollments_.end()) {
        throw NotEnrolledException(studentId_, courseCode);
    }

    (*it)->drop();
    enrollments_.erase(it);
}

void Student::viewTimetable() const {
    timetable_.display();
}

void Student::viewAttendance() const {
    const auto& records = repo::SystemManager::getInstance()
                              .getAttendanceRepository().getItems();

    // Latest record for the same course/session is the effective value. Older
    // entries remain stored so corrections keep their audit history.
    std::map<std::string, repo::AttendanceRecordDTO> latest;
    for (const auto& record : records) {
        if (record.studentId != studentId_) continue;
        const std::string key = record.courseCode + "#" + std::to_string(record.sessionId);
        latest[key] = record;
    }

    std::cout << "\n--- Attendance for " << studentId_ << " ---\n";
    if (latest.empty()) {
        std::cout << "No attendance records found.\n";
        return;
    }

    std::map<std::string, int> totalSessions;
    std::map<std::string, int> presentSessions;
    for (const auto& entry : latest) {
        const auto& record = entry.second;
        std::cout << record.courseCode
                  << " | Session " << record.sessionId
                  << " | " << record.status
                  << " | " << record.captureMethod << "\n";
        ++totalSessions[record.courseCode];
        if (record.status == "Present") ++presentSessions[record.courseCode];
    }

    std::cout << "Total attendance sessions: " << latest.size() << "\n";
    for (const auto& item : totalSessions) {
        const double percentage = item.second == 0 ? 0.0
            : (100.0 * presentSessions[item.first] / item.second);
        std::cout << item.first << " attendance: " << percentage << "%\n";
    }
}

bool Student::isEnrolledIn(const std::string& courseCode) const {
    return std::any_of(enrollments_.begin(), enrollments_.end(),
        [&](const std::shared_ptr<Enrollment>& e) {
            return e->isActive() && e->getCourse()->getCode() == courseCode;
        });
}

const std::string& Student::getStudentId() const noexcept { return studentId_; }

const std::vector<std::shared_ptr<Enrollment>>& Student::getEnrollments() const noexcept {
    return enrollments_;
}

Timetable* Student::getTimetable() noexcept { return &timetable_; }
const Timetable* Student::getTimetable() const noexcept { return &timetable_; }

} // namespace domain
