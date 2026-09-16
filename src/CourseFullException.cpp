#include "CourseFullException.h"

namespace domain {

CourseFullException::CourseFullException(std::string courseCode)
    : LMSException("Course is full: " + courseCode), courseCode_(std::move(courseCode)) {}

const std::string& CourseFullException::getCourseCode() const noexcept {
    return courseCode_;
}

} // namespace domain
