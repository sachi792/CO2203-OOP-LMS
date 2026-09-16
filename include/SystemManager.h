#pragma once
#include "UserRepository.h"
#include "CourseRepository.h"
#include "AttendanceRepository.h"

namespace repo {

// Meyer's singleton (thread-safe static local since C++11), owning the
// three repositories and tracking who is currently logged in.
class SystemManager {
public:
    static SystemManager& getInstance();

    SystemManager(const SystemManager&) = delete;
    SystemManager& operator=(const SystemManager&) = delete;

    // Suggested addition: the UML only lists logout(). A matching login()
    // is needed to actually populate currentUser_ in the first place -
    // without it, logout() has nothing to undo. Delegates the actual
    // credential check to domain::Person::login().
    bool login(const std::string& email, const std::string& password);
    void logout();

    domain::Person* getCurrentUser() const noexcept;

    UserRepository& getUserRepository() noexcept;
    CourseRepository& getCourseRepository() noexcept;
    AttendanceRepository& getAttendanceRepository() noexcept;

private:
    SystemManager() = default;

    UserRepository userRepo_;
    CourseRepository courseRepo_;
    AttendanceRepository attendanceRepo_;
    domain::Person* currentUser_ = nullptr; // non-owning; userRepo_ owns the actual Person objects
};

} // namespace repo
