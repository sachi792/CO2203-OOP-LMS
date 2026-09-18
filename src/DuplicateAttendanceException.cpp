#include "DuplicateAttendanceException.h"

namespace attendance {

DuplicateAttendanceException::DuplicateAttendanceException(std::string studentId, int sessionId)
    : AttendanceException("Student " + studentId + " already has an attendance record for session "
                           + std::to_string(sessionId)),
      studentId_(std::move(studentId)), sessionId_(sessionId) {}

const std::string& DuplicateAttendanceException::getStudentId() const noexcept { return studentId_; }
int DuplicateAttendanceException::getSessionId() const noexcept { return sessionId_; }

}
