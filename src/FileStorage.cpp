#include "FileStorage.h"
#include "StorageException.h"
#include <fstream>
#include <filesystem>

namespace repo {
FileStorage::FileStorage(std::string filePath) : filePath_(std::move(filePath)) {}
void FileStorage::save(const std::vector<std::string>& records) {
    std::ofstream out(filePath_, std::ios::trunc);
    if (!out) throw StorageException("Could not open storage file for writing: " + filePath_);
    for (const auto& record : records) {
        out << record << "\n";
        if (!out) throw StorageException("Write failed for storage file: " + filePath_);
    }
}
std::vector<std::string> FileStorage::load() {
    std::vector<std::string> records;
    if (!std::filesystem::exists(filePath_)) return records; // normal first run
    std::ifstream in(filePath_);
    if (!in) throw StorageException("Could not open storage file for reading: " + filePath_);
    std::string line;
    while (std::getline(in, line)) if (!line.empty()) records.push_back(line);
    if (in.bad()) throw StorageException("Read failed for storage file: " + filePath_);
    return records;
}
const std::string& FileStorage::getFilePath() const noexcept { return filePath_; }
} // namespace repo
