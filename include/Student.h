#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Person.h"

namespace attendance { class Timetable; }

namespace domain {

class Course;
class Enrollment;

// Now that all three modules are merged, this refers to the real
// attendance::Timetable (previously a same-named but never-defined
// domain-namespace stub in Stubs.h - see Stubs.h's own comment, which
// flagged this exact swap as the integration step).
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

    // --- Interface exposed to Member 2 (suggested addition, see project notes) ---
    // Lets the attendance-capture code confirm a student is entitled to be
    // marked present in a given course's session, without needing to know
    // anything about Enrollment internals.
    bool isEnrolledIn(const std::string& courseCode) const;

    const std::string& getStudentId() const noexcept;
    const std::vector<std::shared_ptr<Enrollment>>& getEnrollments() const noexcept;

    void setTimetable(Timetable* timetable) noexcept;
    Timetable* getTimetable() const noexcept;

private:
    std::string studentId_;
    std::vector<std::shared_ptr<Enrollment>> enrollments_;
    Timetable* timetable_ = nullptr; // non-owning; Member 2 owns the real Timetable instance
};

} // namespace domain
