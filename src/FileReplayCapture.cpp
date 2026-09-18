#include "FileReplayCapture.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cctype>
#include <algorithm>

namespace attendance {
FileReplayCapture::FileReplayCapture(std::string logFile) : logFile_(std::move(logFile)) {}
void FileReplayCapture::beginSession() {
    lines_.clear(); replayIndex_ = 0;
    std::ifstream in(logFile_);
    if (!in) throw std::runtime_error("FileReplayCapture could not open: " + logFile_);
    std::string line;
    while (std::getline(in, line)) lines_.push_back(line);
    std::cout << "[FileReplayCapture] Loaded " << lines_.size() << " event(s) from " << logFile_ << "\n";
}
std::optional<std::string> FileReplayCapture::captureNext() {
    if (replayIndex_ >= static_cast<int>(lines_.size())) return std::nullopt;
    const int lineNo = replayIndex_ + 1;
    std::string uid = lines_[replayIndex_++];
    const bool valid = !uid.empty() && uid.size() <= 64 &&
        std::all_of(uid.begin(), uid.end(), [](unsigned char c){ return std::isalnum(c) || c=='_' || c=='-'; });
    if (!valid) throw std::runtime_error("Malformed replay event at line " + std::to_string(lineNo) + ": " + uid);
    return uid;
}
void FileReplayCapture::endSession() {
    std::cout << "[FileReplayCapture] Replay finished (" << replayIndex_ << "/" << lines_.size() << " read)\n";
}
}
