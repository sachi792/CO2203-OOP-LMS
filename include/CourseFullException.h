#pragma once
#include "LMSException.h"

namespace domain {

// Used when a student tries to enroll in a full course.
class CourseFullException : public LMSException {
public:
    explicit CourseFullException(std::string courseCode);

    const std::string& getCourseCode() const noexcept;

private:
    std::string courseCode_;
};

}
