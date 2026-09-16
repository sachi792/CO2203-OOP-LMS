#pragma once
#include "Repository.h"
#include "Person.h"
#include <memory>
#include <string>

namespace repo {

// Repository<std::shared_ptr<domain::Person>>, with a default line-based
// serializer that discriminates Student/Lecturer/Administrator by a leading
// type tag. See UserRepository.cpp for the exact record format.
class UserRepository : public Repository<std::shared_ptr<domain::Person>> {
public:
    explicit UserRepository(const std::string& filePath = "users.dat");

    // Convenience lookups on top of the base add()/getItems().
    std::shared_ptr<domain::Person> findByEmail(const std::string& email) const;
    std::shared_ptr<domain::Person> findById(const std::string& personId) const;

private:
    static std::string toRecord(const std::shared_ptr<domain::Person>& person);
    static std::shared_ptr<domain::Person> fromRecord(const std::string& record);
};

} // namespace repo
