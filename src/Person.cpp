#include "Person.h"
#include <ostream>

namespace domain {

Person::Person(std::string personId, std::string name, std::string email, std::string password)
    : personId_(std::move(personId)), name_(std::move(name)),
      email_(std::move(email)), password_(std::move(password)) {}

bool Person::login(const std::string& emailAttempt, const std::string& passwordAttempt) const {
    return email_ == emailAttempt && password_ == passwordAttempt;
}

Menu Person::getMenu() const {
    // Placeholder until the role-based menu is connected.
    return Menu{};
}

const std::string& Person::getPersonId() const noexcept { return personId_; }
const std::string& Person::getName() const noexcept { return name_; }
const std::string& Person::getEmail() const noexcept { return email_; }

std::ostream& operator<<(std::ostream& os, const Person& person) {
    return os << person.getPersonId() << " | " << person.getName() << " | " << person.getEmail();
}

}

namespace domain {
const std::string& Person::getPassword() const noexcept { return password_; }
void Person::setName(const std::string& name) { name_ = name; }
void Person::setEmail(const std::string& email) { email_ = email; }
void Person::setPassword(const std::string& password) { password_ = password; }
}
