#pragma once
#include <string>

// need only the Student declaration here since we store a pointer.
namespace domain { class Student; }

namespace attendance {

// Connects a card UID with a student.
class StudentCard {
public:
    StudentCard(std::string uid, domain::Student* owner);

    const std::string& getUID() const noexcept;
    domain::Student* getOwner() const noexcept;

private:
    std::string uid_;
    domain::Student* owner_;    // student is managed somewhere else
};

}