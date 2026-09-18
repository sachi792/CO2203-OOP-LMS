#include "SessionClosedException.h"

namespace attendance {

SessionClosedException::SessionClosedException(int sessionId)
    : AttendanceException("Attendance session " + std::to_string(sessionId) + " is not open"),
      sessionId_(sessionId) {}

int SessionClosedException::getSessionId() const noexcept { return sessionId_; }

}
