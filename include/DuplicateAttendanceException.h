#pragma once
#include "AttendanceException.h"

namespace attendance {

// Thrown when AttendanceRegister::markAttendance() finds the student
// already has a record for this exact session.
class DuplicateAttendanceException : public AttendanceException {
public:
    DuplicateAttendanceException(std::string studentId, int sessionId);

    const std::string& getStudentId() const noexcept;
    int getSessionId() const noexcept;

private:
    std::string studentId_;
    int sessionId_;
};

} // namespace attendance
