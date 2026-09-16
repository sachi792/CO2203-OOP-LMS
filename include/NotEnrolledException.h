#pragma once
#include "LMSException.h"

namespace domain {

// Thrown when Enrollment::drop() (or Student::dropCourse()) is called
// for a course the student is not currently, actively enrolled in.
class NotEnrolledException : public LMSException {
public:
    NotEnrolledException(std::string studentId, std::string courseCode);

    const std::string& getStudentId() const noexcept;
    const std::string& getCourseCode() const noexcept;

private:
    std::string studentId_;
    std::string courseCode_;
};

} // namespace domain
