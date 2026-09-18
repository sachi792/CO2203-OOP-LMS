#pragma once
#include "Repository.h"
#include "Person.h"
#include <memory>
#include <string>

namespace repo {

// Repository for storing Person objects.
class UserRepository : public Repository<std::shared_ptr<domain::Person>> {
public:
    explicit UserRepository(const std::string& filePath = "users.dat");

    // Find a user using their email or ID.
    std::shared_ptr<domain::Person> findByEmail(const std::string& email) const;
    std::shared_ptr<domain::Person> findById(const std::string& personId) const;

private:
    // Converts a user to a string for saving.
    static std::string toRecord(const std::shared_ptr<domain::Person>& person);

    // Converts a saved string back into a user object.
    static std::shared_ptr<domain::Person> fromRecord(const std::string& record);
};

}
