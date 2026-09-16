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

    void openAttendance();          // delegates to Member 2's attendance subsystem
    void markAttendance();          // delegates to Member 2's attendance subsystem
    void viewCourses() const;
    void selectCaptureMethod();     // delegates to Member 2's AttendanceCapture hierarchy

    const std::string& getDepartment() const noexcept;
    const std::string& getStaffId() const noexcept;

    void assignCourse(Course* course);
    const std::vector<Course*>& getCourses() const noexcept;

private:
    std::string department_;
    std::string staffId_;
    std::vector<Course*> courses_; // non-owning
};

} // namespace domain
