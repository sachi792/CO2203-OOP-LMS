#pragma once
#include <string>
#include <iosfwd>
#include "Stubs.h"

namespace domain {

// Base class for Student, Lecturer and Administrator.
// Each role has its own dashboard.
class Person {
public:
    Person(std::string personId, std::string name, std::string email, std::string password);
    virtual ~Person() = default;

    // Default copy and move are enough since this class only has value members.
    Person(const Person&) = default;
    Person& operator=(const Person&) = default;
    Person(Person&&) noexcept = default;
    Person& operator=(Person&&) noexcept = default;

    // Checks the email and password.
    bool login(const std::string& emailAttempt, const std::string& passwordAttempt) const;

    // Each role provides its own dashboard.
    virtual Dashboard getDashboard() const = 0;

    // Common menu for all roles.
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

}