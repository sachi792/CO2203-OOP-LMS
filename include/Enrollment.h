#pragma once

namespace domain {

class Student;
class Course;

// Connects a student with a course and handles the enrollment rules.
class Enrollment {
public:
    Enrollment(Student& student, Course& course);

    void enroll(); // throws CourseFullException, PrerequisiteException, TimetableClashException, NotEnrolledException
    void drop();   

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

}