#include "Enrollment.h"
#include "Student.h"
#include "Course.h"
#include "PrerequisiteException.h"
#include "NotEnrolledException.h"
#include "TimetableClashException.h"
#include "Timetable.h" // concrete type needed here now that all modules are merged;
                        // Enrollment's own header still only forward-declares Timetable
                        // via Stubs.h, keeping that dependency out of the public interface
#include <algorithm>

namespace domain {

Enrollment::Enrollment(Student& student, Course& course)
    : student_(&student), course_(&course) {}

void Enrollment::enroll() {
    // Order matters: cheapest/most-common failure checked first.
    checkPrerequisites();  // throws PrerequisiteException
    checkTimetableClash(); // throws TimetableClashException

    course_->addStudent(*student_); // throws CourseFullException
    active_ = true;

    // The clash check only works on FUTURE registrations if this course's
    // slot is actually added to the student's timetable now - otherwise
    // every check would compare against an empty/stale timetable.
    if (attendance::Timetable* timetable = student_->getTimetable()) {
        timetable->addSlot(course_->getSchedule());
    }
}

void Enrollment::drop() {
    if (!active_) {
        throw NotEnrolledException(student_->getStudentId(), course_->getCode());
    }
    course_->removeStudent(*student_);
    if (attendance::Timetable* timetable = student_->getTimetable()) {
        timetable->removeSlot(course_->getSchedule());
    }
    active_ = false;
}

Student* Enrollment::getStudent() const noexcept { return student_; }
Course* Enrollment::getCourse() const noexcept { return course_; }
bool Enrollment::isActive() const noexcept { return active_; }

void Enrollment::checkPrerequisites() const {
    for (Course* prereq : course_->getPrerequisites()) {
        if (!student_->isEnrolledIn(prereq->getCode())) {
            throw PrerequisiteException(prereq->getCode());
        }
    }
}

void Enrollment::checkTimetableClash() const {
    attendance::Timetable* timetable = student_->getTimetable();
    if (timetable == nullptr) {
        // Student has no Timetable attached yet - nothing to check against.
        // (SystemManager/whoever constructs the Student is responsible for
        // calling setTimetable() - see README for this known gap.)
        return;
    }

    auto clash = timetable->clashesWith(course_->getSchedule());
    if (clash) {
        throw TimetableClashException(*clash, course_->getSchedule());
    }
}

} // namespace domain
