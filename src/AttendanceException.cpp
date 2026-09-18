#include "AttendanceException.h"

namespace attendance {

AttendanceException::AttendanceException(std::string message) : message_(std::move(message)) {}

AttendanceException::~AttendanceException() = default;

const char* AttendanceException::what() const noexcept {
    return message_.c_str();
}

}
