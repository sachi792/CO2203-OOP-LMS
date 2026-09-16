#pragma once
#include "LMSException.h"

namespace domain {

// Thrown when Enrollment::enroll() finds a required prerequisite Course
// has not been completed/enrolled by the Student.
class PrerequisiteException : public LMSException {
public:
    explicit PrerequisiteException(std::string missingPrereq);

    const std::string& getMissingPrereq() const noexcept;

private:
    std::string missingPrereq_;
};

} // namespace domain
