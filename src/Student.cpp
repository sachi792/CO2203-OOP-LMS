#include "Student.h"
#include "Course.h"
#include "Enrollment.h"
#include "NotEnrolledException.h"
#include <algorithm>
#include <iostream>

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
    // Delegates to Member 2's Timetable once its interface is available.
    std::cout << "[Student " << studentId_ << "] viewTimetable() - delegates to Member 2's Timetable\n";
}

void Student::viewAttendance() const {
    // Delegates to Member 2's AttendanceRegister once its interface is available.
    std::cout << "[Student " << studentId_ << "] viewAttendance() - delegates to Member 2's AttendanceRegister\n";
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

void Student::setTimetable(Timetable* timetable) noexcept { timetable_ = timetable; }
Timetable* Student::getTimetable() const noexcept { return timetable_; }

} // namespace domain
