#include "Course.h"
#include "CourseFullException.h"
#include <algorithm>

namespace domain {

Course::Course(std::string courseCode, std::string title, int creditValue, int capacity)
    : courseCode_(std::move(courseCode)), title_(std::move(title)),
      creditValue_(creditValue), capacity_(capacity) {}

const std::string& Course::getCode() const noexcept { return courseCode_; }
const std::string& Course::getTitle() const noexcept { return title_; }
int Course::getCredits() const noexcept { return creditValue_; }

void Course::addStudent(Student& student) {
    if (isFull()) {
        throw CourseFullException(courseCode_);
    }
    enrolledStudents_.push_back(&student);
}

void Course::removeStudent(const Student& student) {
    enrolledStudents_.erase(
        std::remove(enrolledStudents_.begin(), enrolledStudents_.end(), &student),
        enrolledStudents_.end());
}

bool Course::isFull() const noexcept {
    return static_cast<int>(enrolledStudents_.size()) >= capacity_;
}

void Course::setLecturer(Lecturer* lecturer) noexcept { lecturer_ = lecturer; }
Lecturer* Course::getLecturer() const noexcept { return lecturer_; }

void Course::addPrerequisite(Course* prerequisite) {
    if (prerequisite != nullptr) {
        prerequisites_.push_back(prerequisite);
    }
}

const std::vector<Course*>& Course::getPrerequisites() const noexcept { return prerequisites_; }

const std::vector<Student*>& Course::getEnrolledStudents() const noexcept {
    return enrolledStudents_;
}

void Course::setSchedule(const TimeSlot& slot) noexcept { schedule_ = slot; }
const TimeSlot& Course::getSchedule() const noexcept { return schedule_; }

} // namespace domain
