#pragma once
#include "AttendanceException.h"

namespace attendance {

// Used when attendance is marked for a session that is not open.
class SessionClosedException : public AttendanceException {
public:
    explicit SessionClosedException(int sessionId);

    int getSessionId() const noexcept;

private:
    int sessionId_;
};

}
