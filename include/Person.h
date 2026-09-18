#pragma once
#include <string>
#include <iosfwd>
#include "Stubs.h"

namespace domain {

// Abstract base of the Person hierarchy (Student / Lecturer / Administrator).
// Abstract because getDashboard() is pure virtual: every concrete role
// presents a different dashboard, so Person alone cannot be instantiated.
class Person {
public:
    Person(std::string personId, std::string name, std::string email, std::string password);
    virtual ~Person() = default;

    // Only value members owned here -> compiler-generated copy/move are
    // correct and sufficient (Rule of Zero applies at this level).
    Person(const Person&) = default;
    Person& operator=(const Person&) = default;
    Person(Person&&) noexcept = default;
    Person& operator=(Person&&) noexcept = default;

    // Common credential check shared by every role.
    bool login(const std::string& emailAttempt, const std::string& passwordAttempt) const;

    // Pure virtual: forces every concrete Person to supply its own dashboard.
    virtual Dashboard getDashboard() const = 0;

    // Not overridden per-role in the UML, so a single shared implementation
    // lives here. Still virtual in case a future role needs to customise it.
    virtual Menu getMenu() const;

    friend std::ostream& operator<<(std::ostream& os, const Person& person);

    const std::string& getPersonId() const noexcept;
    const std::string& getName() const noexcept;
    const std::string& getEmail() const noexcept;
    const std::string& getPassword() const noexcept;
    void setName(const std::string& name);
    void setEmail(const std::string& email);
    void setPassword(const std::string& password);

protected:
    std::string personId_;
    std::string name_;
    std::string email_;
    std::string password_;
};

} // namespace domain
