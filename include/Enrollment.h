#pragma once

namespace domain {

class Student;
class Course;

// Association class linking exactly one Student to exactly one Course.
// This is where enrolment BUSINESS RULES live (capacity, prerequisites,
// timetable clash) rather than inside Student or Course themselves -
// keeping those two classes focused on their own data.
class Enrollment {
public:
    Enrollment(Student& student, Course& course);

    void enroll(); // throws CourseFullException, PrerequisiteException, TimetableClashException
    void drop();   // throws NotEnrolledException

    Student* getStudent() const noexcept;
    Course* getCourse() const noexcept;
    bool isActive() const noexcept;

private:
    Student* student_;
    Course* course_;
    bool active_ = false;

    void checkPrerequisites() const;   // throws PrerequisiteException
    void checkTimetableClash() const;  // throws TimetableClashException
};

} // namespace domain
