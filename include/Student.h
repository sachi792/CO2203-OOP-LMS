#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Person.h"
#include "Timetable.h"

namespace domain {

class Course;
class Enrollment;

// Use the Timetable from the attendance module.
using attendance::Timetable;

class Student : public Person {
public:
    Student(std::string personId, std::string name, std::string email, std::string password,
            std::string studentId);

    Dashboard getDashboard() const override;

    // May throw CourseFullException, PrerequisiteException, TimetableClashException
    void registerCourse(Course& course);

    // May throw NotEnrolledException
    void dropCourse(const std::string& courseCode);

    void viewTimetable() const;
    void viewAttendance() const;

    // Used by the attendance module to check if the student is enrolled.
    bool isEnrolledIn(const std::string& courseCode) const;

    const std::string& getStudentId() const noexcept;
    const std::vector<std::shared_ptr<Enrollment>>& getEnrollments() const noexcept;

    Timetable* getTimetable() noexcept;
    const Timetable* getTimetable() const noexcept;

private:
    std::string studentId_;
    std::vector<std::shared_ptr<Enrollment>> enrollments_;
    Timetable timetable_;
};

} 