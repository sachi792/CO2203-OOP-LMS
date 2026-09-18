#pragma once
#include "AttendanceException.h"

namespace attendance {

// Used when attendance is already marked for the same session.
class DuplicateAttendanceException : public AttendanceException {
public:
    DuplicateAttendanceException(std::string studentId, int sessionId);

    const std::string& getStudentId() const noexcept;
    int getSessionId() const noexcept;

private:
    std::string studentId_;
    int sessionId_;
};

} 