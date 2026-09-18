#pragma once
#include <string>
#include <ctime>

namespace repo {

// Store IDs instead of pointers so the data can be saved and loaded later.
// repository works with DTOs datas

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

} 
