#pragma once
#include "LMSException.h"

namespace domain {

// Used when a student tries to drop a course they are not enrolled in.
class NotEnrolledException : public LMSException {
public:
    NotEnrolledException(std::string studentId, std::string courseCode);

    const std::string& getStudentId() const noexcept;
    const std::string& getCourseCode() const noexcept;

private:
    std::string studentId_;
    std::string courseCode_;
};

} 