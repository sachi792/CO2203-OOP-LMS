#include "Person.h"

namespace domain {

Person::Person(std::string personId, std::string name, std::string email, std::string password)
    : personId_(std::move(personId)), name_(std::move(name)),
      email_(std::move(email)), password_(std::move(password)) {}

bool Person::login(const std::string& emailAttempt, const std::string& passwordAttempt) const {
    return email_ == emailAttempt && password_ == passwordAttempt;
}

Menu Person::getMenu() const {
    // Placeholder: real menu composition will be driven by Member 3's
    // MenuUI once it is wired up to role-based permissions.
    return Menu{};
}

const std::string& Person::getPersonId() const noexcept { return personId_; }
const std::string& Person::getName() const noexcept { return name_; }
const std::string& Person::getEmail() const noexcept { return email_; }

} // namespace domain
