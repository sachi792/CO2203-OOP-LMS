#pragma once
#include <exception>
#include <string>

namespace attendance {

// Abstract root of Member 2's OWN exception hierarchy - intentionally
// separate from domain::LMSException (Member 1's enrolment exceptions).
// This keeps the two subsystems' error handling independent, per the
// group's agreed exception-hierarchy scoping. Same "pure-virtual
// destructor" trick as LMSException to make the base non-instantiable
// while still sharing real behaviour.
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

} // namespace attendance
