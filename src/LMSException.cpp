#include "LMSException.h"

namespace domain {

LMSException::LMSException(std::string message) : message_(std::move(message)) {}

// Definition is needed even though the destructor is pure virtual.
LMSException::~LMSException() = default;

const char* LMSException::what() const noexcept {
    return message_.c_str();
}

} 
