#pragma once
#include <stdexcept>
#include <string>
namespace repo {
class StorageException : public std::runtime_error {
public:
    explicit StorageException(const std::string& message) : std::runtime_error(message) {}
};
}
