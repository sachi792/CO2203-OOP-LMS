#pragma once
#include "Person.h"
#include <vector>

namespace domain {

class Course;

class Lecturer : public Person {
public:
    Lecturer(std::string personId, std::string name, std::string email, std::string password,
              std::string department, std::string staffId);

    Dashboard getDashboard() const override;

    void openAttendance();          // handles attendance through the attendance module
    void markAttendance();          // handles attendance through the attendance module
    void viewCourses() const;
    void viewEnrollmentList() const;
    void selectCaptureMethod();     // selects the attendance capture method

    // Used by the console UI to control attendance and view its results.
    void closeAttendance();
    void viewAttendanceReport() const;
    void correctAttendance();

    const std::string& getDepartment() const noexcept;
    const std::string& getStaffId() const noexcept;

    void assignCourse(Course* course);
    void removeCourse(const Course* course);
    const std::vector<Course*>& getCourses() const noexcept;

private:
    std::string department_;
    std::string staffId_;
    std::vector<Course*> courses_;
};

} 
