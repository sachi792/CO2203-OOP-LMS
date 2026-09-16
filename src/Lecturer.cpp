#include "Lecturer.h"
#include "Course.h"
#include <iostream>

namespace domain {

Lecturer::Lecturer(std::string personId, std::string name, std::string email, std::string password,
                    std::string department, std::string staffId)
    : Person(std::move(personId), std::move(name), std::move(email), std::move(password)),
      department_(std::move(department)), staffId_(std::move(staffId)) {}

Dashboard Lecturer::getDashboard() const {
    return Dashboard{};
}

void Lecturer::openAttendance() {
    std::cout << "[Lecturer " << staffId_ << "] openAttendance() - delegates to Member 2's AttendanceSession\n";
}

void Lecturer::markAttendance() {
    std::cout << "[Lecturer " << staffId_ << "] markAttendance() - delegates to Member 2's AttendanceRegister\n";
}

void Lecturer::viewCourses() const {
    std::cout << "[Lecturer " << staffId_ << "] teaches " << courses_.size() << " course(s)\n";
}

void Lecturer::selectCaptureMethod() {
    std::cout << "[Lecturer " << staffId_ << "] selectCaptureMethod() - delegates to Member 2's AttendanceCapture hierarchy\n";
}

const std::string& Lecturer::getDepartment() const noexcept { return department_; }
const std::string& Lecturer::getStaffId() const noexcept { return staffId_; }

void Lecturer::assignCourse(Course* course) {
    if (course != nullptr) {
        courses_.push_back(course);
        course->setLecturer(this);
    }
}

const std::vector<Course*>& Lecturer::getCourses() const noexcept { return courses_; }

} // namespace domain
