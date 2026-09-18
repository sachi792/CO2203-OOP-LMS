#pragma once
#include "LMSException.h"

namespace domain {

// Used when a required prerequisite is missing.
class PrerequisiteException : public LMSException {
public:
    explicit PrerequisiteException(std::string missingPrereq);

    const std::string& getMissingPrereq() const noexcept;

private:
    std::string missingPrereq_;
};

} 