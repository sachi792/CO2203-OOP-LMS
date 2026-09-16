#include "FileReplayCapture.h"
#include <fstream>
#include <iostream>

namespace attendance {

FileReplayCapture::FileReplayCapture(std::string logFile) : logFile_(std::move(logFile)) {}

void FileReplayCapture::beginSession() {
    lines_.clear();
    replayIndex_ = 0;

    std::ifstream in(logFile_);
    if (!in) {
        std::cout << "[FileReplayCapture] Could not open log file: " << logFile_ << "\n";
        return;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) {
            lines_.push_back(line);
        }
    }
    std::cout << "[FileReplayCapture] Loaded " << lines_.size() << " UID(s) from " << logFile_ << "\n";
}

std::optional<std::string> FileReplayCapture::captureNext() {
    if (replayIndex_ >= static_cast<int>(lines_.size())) {
        return std::nullopt;
    }
    return lines_[replayIndex_++];
}

void FileReplayCapture::endSession() {
    std::cout << "[FileReplayCapture] Replay finished (" << replayIndex_ << "/" << lines_.size() << " read)\n";
}

} // namespace attendance
