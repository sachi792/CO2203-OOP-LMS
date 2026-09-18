#pragma once
#include "IStorage.h"
#include <string>

namespace repo {

// Concrete IStorage that persists records as plain lines in a text file.
class FileStorage : public IStorage {
public:
    explicit FileStorage(std::string filePath);

    void save(const std::vector<std::string>& records) override;
    std::vector<std::string> load() override;

    const std::string& getFilePath() const noexcept;

private:
    std::string filePath_;
};

} 
