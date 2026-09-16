#pragma once
#include <vector>
#include <string>

namespace repo {

// Storage abstraction. Deliberately NOT templated on domain types - it only
// ever deals in opaque string "records" (one per line). This keeps storage
// technology (files today, maybe a database later) completely decoupled
// from domain knowledge: IStorage doesn't need to know what a Student or
// Course is, only how to persist and retrieve lines of text.
//
// Suggested UML addition: the original diagram shows FileStorage directly,
// with no interface above it. Introducing IStorage here means a future
// DatabaseStorage/CloudStorage could be swapped in without Repository or
// any domain code changing - classic Dependency Inversion.
class IStorage {
public:
    virtual ~IStorage() = default;

    virtual void save(const std::vector<std::string>& records) = 0;
    virtual std::vector<std::string> load() = 0;
};

} // namespace repo
