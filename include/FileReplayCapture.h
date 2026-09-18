#pragma once
#include "AttendanceCapture.h"
#include <string>
#include <vector>

namespace attendance {

// Reads card UIDs from a file instead of using a real card reader.
class FileReplayCapture : public AttendanceCapture {
public:
    explicit FileReplayCapture(std::string logFile);

    void beginSession() override; 
    std::optional<std::string> captureNext() override;
    void endSession() override;

private:
    std::string logFile_;
    int replayIndex_ = 0;
    std::vector<std::string> lines_;
};

} 
