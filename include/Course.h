#pragma once
#include <string>
#include <vector>
#include "Stubs.h"
#include "TimeSlot.h"

namespace domain {

class Lecturer;
class Student;

// TimeSlot canonically lives in the attendance module (see
// CO2203_Attendance/README.md, suggestion #5).
using attendance::TimeSlot;

// Abstract base of the Course hierarchy (LectureBasedCourse / LabBasedCourse /
// ProjectBasedCourse). Abstract because calculateGrade() is pure virtual:
// each course type grades differently.
class Course {
public:
    Course(std::string courseCode, std::string title, int creditValue, int capacity);
    virtual ~Course() = default;

    Course(const Course&) = delete;            // Course identity shouldn't be copied;
    Course& operator=(const Course&) = delete;  // Student/Course* pointers rely on stable addresses.
    Course(Course&&) = delete;
    Course& operator=(Course&&) = delete;

    virtual Grade calculateGrade(double marks) const = 0;

    const std::string& getCode() const noexcept;
    const std::string& getTitle() const noexcept;
    int getCredits() const noexcept;

    void addStudent(Student& student);          // throws CourseFullException
    void removeStudent(const Student& student);
    bool isFull() const noexcept;

    void setLecturer(Lecturer* lecturer) noexcept;
    Lecturer* getLecturer() const noexcept;

    void addPrerequisite(Course* prerequisite);
    const std::vector<Course*>& getPrerequisites() const noexcept;

    const std::vector<Student*>& getEnrolledStudents() const noexcept;

    // Added during final integration (not in the original UML - flagged):
    // Enrollment::checkTimetableClash() needs to know WHEN a course meets
    // to compare it against a student's existing Timetable. Without this,
    // there is nothing to check a clash against. See README.
    void setSchedule(const TimeSlot& slot) noexcept;
    const TimeSlot& getSchedule() const noexcept;

protected:
    std::string courseCode_;
    std::string title_;
    int creditValue_;
    int capacity_;
    Lecturer* lecturer_ = nullptr;              // non-owning; Lecturer lives in Person hierarchy
    std::vector<Course*> prerequisites_;         // non-owning references to other Courses
    std::vector<Student*> enrolledStudents_;     // non-owning references
    TimeSlot schedule_;                          // default-constructed = "unscheduled"
};

} // namespace domain
