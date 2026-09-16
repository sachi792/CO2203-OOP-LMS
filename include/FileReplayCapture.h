#pragma once
#include "AttendanceCapture.h"
#include <string>
#include <vector>

namespace attendance {

// Replays a pre-recorded log of card UIDs from a file - useful for demos,
// automated testing, and simulating a session without live hardware.
class FileReplayCapture : public AttendanceCapture {
public:
    explicit FileReplayCapture(std::string logFile);

    void beginSession() override; // loads the file into memory
    std::optional<std::string> captureNext() override;
    void endSession() override;

private:
    std::string logFile_;
    int replayIndex_ = 0;
    std::vector<std::string> lines_;
};

} // namespace attendance
