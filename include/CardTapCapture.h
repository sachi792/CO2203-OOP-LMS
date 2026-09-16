#pragma once
#include "AttendanceCapture.h"
#include "ConsoleCardReader.h"

namespace attendance {

// Live capture: uses a ConsoleCardReader ("1 1 uses" in the UML) to read
// one card UID at a time as the lecturer's session runs.
class CardTapCapture : public AttendanceCapture {
public:
    void beginSession() override;
    std::optional<std::string> captureNext() override;
    void endSession() override;

private:
    ConsoleCardReader reader_;
};

} // namespace attendance
