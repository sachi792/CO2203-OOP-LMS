#include "FileStorage.h"
#include <fstream>
#include <iostream>

namespace repo {

FileStorage::FileStorage(std::string filePath) : filePath_(std::move(filePath)) {}

void FileStorage::save(const std::vector<std::string>& records) {
    std::ofstream out(filePath_, std::ios::trunc);
    if (!out) {
        std::cerr << "[FileStorage] Could not open for writing: " << filePath_ << "\n";
        return;
    }
    for (const auto& record : records) {
        out << record << "\n";
    }
}

std::vector<std::string> FileStorage::load() {
    std::vector<std::string> records;
    std::ifstream in(filePath_);
    if (!in) {
        // Not an error - a repository that has never been saved yet simply
        // has no file on disk. Return an empty result.
        return records;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) {
            records.push_back(line);
        }
    }
    return records;
}

const std::string& FileStorage::getFilePath() const noexcept { return filePath_; }

} // namespace repo
