#pragma once
#include <string>
#include <vector>
#include <iosfwd>
#include "Stubs.h"
#include "TimeSlot.h"

namespace domain {

class Lecturer;
class Student;

// TimeSlot defined in the attendance module
using attendance::TimeSlot;

// Base class for the different course types.
// Each course type has specific method of calculating grades.
class Course {
public:
    Course(std::string courseCode, std::string title, int creditValue, int capacity);
    virtual ~Course() = default;

    Course(const Course&) = delete;                 // Keep course identity unique.
    Course& operator=(const Course&) = delete;      // Student/Course pointers need stable addresses.
    Course(Course&&) = delete;
    Course& operator=(Course&&) = delete;

    virtual Grade calculateGrade(double marks) const = 0;

    const std::string& getCode() const noexcept;
    const std::string& getTitle() const noexcept;
    int getCredits() const noexcept;
    int getCapacity() const noexcept;
    void setTitle(const std::string& title);
    void setCredits(int credits) noexcept;
    void setCapacity(int capacity) noexcept;

    void addStudent(Student& student);       
    void removeStudent(const Student& student);
    bool isFull() const noexcept;

    void setLecturer(Lecturer* lecturer) noexcept;
    Lecturer* getLecturer() const noexcept;

    void addPrerequisite(Course* prerequisite);
    void removePrerequisite(const Course* prerequisite);
    const std::vector<Course*>& getPrerequisites() const noexcept;

    const std::vector<Student*>& getEnrolledStudents() const noexcept;

    // Stores the course meeting time so timetable clashes can be checked.
    void setSchedule(const TimeSlot& slot) noexcept;
    const TimeSlot& getSchedule() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const Course& course);

protected:
    std::string courseCode_;
    std::string title_;
    int creditValue_;
    int capacity_;
    Lecturer* lecturer_ = nullptr;              
    std::vector<Course*> prerequisites_;         
    std::vector<Student*> enrolledStudents_;   
    TimeSlot schedule_;                        
};

} 
