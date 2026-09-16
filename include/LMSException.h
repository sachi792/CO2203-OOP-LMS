#pragma once
#include <exception>
#include <string>

namespace domain {

// Abstract base of ALL enrolment-related exceptions in Member 1's subsystem.
// Made abstract via a pure-virtual destructor: it provides real, shared
// behaviour (storing/returning the message) while still preventing anyone
// from throwing a bare LMSException.
//
// Scope note: Attendance-side exceptions (DuplicateAttendanceException,
// SessionClosedException) belong to Member 2 and derive from their OWN
// attendance exception hierarchy, not from this one - keeping the two
// subsystems' error handling independent, as agreed.
class LMSException : public std::exception {
public:
    explicit LMSException(std::string message);
    ~LMSException() override = 0; // pure virtual destructor -> class is abstract

    LMSException(const LMSException&) = default;
    LMSException& operator=(const LMSException&) = default;
    LMSException(LMSException&&) noexcept = default;
    LMSException& operator=(LMSException&&) noexcept = default;

    const char* what() const noexcept override;

protected:
    std::string message_;
};

} // namespace domain
