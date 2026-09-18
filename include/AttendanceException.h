#pragma once
#include <exception>
#include <string>

namespace attendance {

// Base exception class for Member 2 errors.
// kept separate from LMSException used for enrolment errors,

class AttendanceException : public std::exception {
public:
    explicit AttendanceException(std::string message);
    ~AttendanceException() override = 0;

    AttendanceException(const AttendanceException&) = default;
    AttendanceException& operator=(const AttendanceException&) = default;
    AttendanceException(AttendanceException&&) noexcept = default;
    AttendanceException& operator=(AttendanceException&&) noexcept = default;

    const char* what() const noexcept override;

protected:
    std::string message_;
};

} 
