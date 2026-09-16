#pragma once
#include <string>

// Forward declaration of Member 1's Student class - we only ever need a
// pointer to it here, so we avoid a hard #include dependency where possible.
// (main_demo.cpp / AttendanceRegister.cpp will #include the real Student.h.)
namespace domain { class Student; }

namespace attendance {

// Maps a physical/simulated card UID to the Student who owns it, so
// CardTapCapture's raw UID reads can be resolved to a real domain::Student.
class StudentCard {
public:
    StudentCard(std::string uid, domain::Student* owner);

    const std::string& getUID() const noexcept;
    domain::Student* getOwner() const noexcept;

private:
    std::string uid_;
    domain::Student* owner_; // non-owning; Student's lifetime is managed elsewhere
};

} // namespace attendance
