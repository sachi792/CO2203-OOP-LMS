#pragma once
#include "AttendanceException.h"

namespace attendance {

// Thrown when AttendanceRegister::markAttendance() is called against a
// session that is not open (never opened, already closed, or expired).
class SessionClosedException : public AttendanceException {
public:
    explicit SessionClosedException(int sessionId);

    int getSessionId() const noexcept;

private:
    int sessionId_;
};

} // namespace attendance
