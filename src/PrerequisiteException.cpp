#include "PrerequisiteException.h"

namespace domain {

PrerequisiteException::PrerequisiteException(std::string missingPrereq)
    : LMSException("Missing prerequisite: " + missingPrereq),
      missingPrereq_(std::move(missingPrereq)) {}

const std::string& PrerequisiteException::getMissingPrereq() const noexcept {
    return missingPrereq_;
}

}
