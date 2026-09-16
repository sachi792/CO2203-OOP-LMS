#pragma once
#include <string>
#include <ctime>

namespace repo {

// AttendanceRecordDTO deliberately does NOT hold pointers to a live
// domain::Student or attendance::AttendanceSession the way
// attendance::AttendanceRecord does - pointers aren't meaningful once
// written to a file and read back in a different run of the program.
// Instead it stores IDs, which is what makes it persistable.
//
// Flagged as an open design question (see README): reconstructing a live
// attendance::AttendanceRecord from this DTO after loadAll() would need a
// lookup against UserRepository (by studentId) and some session registry
// (by sessionId) - that registry doesn't exist yet anywhere in the project.
// For now, AttendanceRepository persists and retrieves DTOs only.
struct AttendanceRecordDTO {
    int sessionId = 0;
    std::string courseCode;
    std::string studentId;
    std::string status;
    std::time_t timestamp = 0;
    std::string captureMethod;

    bool operator==(const AttendanceRecordDTO& other) const noexcept {
        return sessionId == other.sessionId && studentId == other.studentId &&
               timestamp == other.timestamp;
    }
};

} // namespace repo
