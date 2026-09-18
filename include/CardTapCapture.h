#pragma once
#include "AttendanceCapture.h"
#include "ConsoleCardReader.h"

namespace attendance {

// Uses the card reader to get student IDs during a live session.
class CardTapCapture : public AttendanceCapture {
public:
    void beginSession() override;
    std::optional<std::string> captureNext() override;
    void endSession() override;

private:
    ConsoleCardReader reader_;
};

} 
