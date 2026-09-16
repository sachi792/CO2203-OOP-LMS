#pragma once
#include <optional>
#include <string>

namespace attendance {

// Abstract strategy for capturing attendance. Concrete subclasses decide
// HOW a student is identified (card tap, replaying a log file, etc);
// AttendanceRegister::markAttendance() decides WHAT happens once a
// student's identity has been captured.
class AttendanceCapture {
public:
    virtual ~AttendanceCapture() = 0;

    virtual void beginSession() = 0;

    // Returns the next captured student identifier (e.g. a card UID), or
    // std::nullopt when there is nothing left to capture (reader idle /
    // end of replay log / session manually ended).
    virtual std::optional<std::string> captureNext() = 0;

    virtual void endSession() = 0;
};

} // namespace attendance
