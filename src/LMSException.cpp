#include "LMSException.h"

namespace domain {

LMSException::LMSException(std::string message) : message_(std::move(message)) {}

// Out-of-line definition required even for a pure-virtual destructor.
LMSException::~LMSException() = default;

const char* LMSException::what() const noexcept {
    return message_.c_str();
}

} // namespace domain
