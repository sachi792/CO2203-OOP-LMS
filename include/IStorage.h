#pragma once
#include <vector>
#include <string>

namespace repo {

// only deals with strings, so it doesn't need to know about students, courses or other domain classes.

class IStorage {
public:
    virtual ~IStorage() = default;

    virtual void save(const std::vector<std::string>& records) = 0;
    virtual std::vector<std::string> load() = 0;
};

}