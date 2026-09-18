#pragma once
#include <optional>
#include <string>

namespace attendance {

class AttendanceCapture {
public:
    virtual ~AttendanceCapture() = 0;

    virtual void beginSession() = 0;

 // Get the next student ID, like a card UID.
// Returns std::nullopt if there is nothing more to read,
    virtual std::optional<std::string> captureNext() = 0;

    virtual void endSession() = 0;
};

}
