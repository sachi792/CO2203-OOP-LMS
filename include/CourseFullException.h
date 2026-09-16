#pragma once
#include "LMSException.h"

namespace domain {

// Thrown when Enrollment::enroll() finds Course::isFull() == true.
class CourseFullException : public LMSException {
public:
    explicit CourseFullException(std::string courseCode);

    const std::string& getCourseCode() const noexcept;

private:
    std::string courseCode_;
};

} // namespace domain
