#include "SystemManager.h"

namespace repo {

SystemManager::SystemManager() {
    // Attendance is persisted by Member 3's repository/storage layer.
    // Reload it when the application starts so Student::viewAttendance()
    // can see records created during earlier program runs.
    attendanceRepo_.loadAll();
}

SystemManager& SystemManager::getInstance() {
    static SystemManager instance; // constructed on first use, destroyed at program exit
    return instance;
}

bool SystemManager::login(const std::string& email, const std::string& password) {
    auto person = userRepo_.findByEmail(email);
    if (person && person->login(email, password)) {
        currentUser_ = person.get();
        return true;
    }
    return false;
}

void SystemManager::logout() {
    currentUser_ = nullptr;
}

domain::Person* SystemManager::getCurrentUser() const noexcept { return currentUser_; }

UserRepository& SystemManager::getUserRepository() noexcept { return userRepo_; }
CourseRepository& SystemManager::getCourseRepository() noexcept { return courseRepo_; }
AttendanceRepository& SystemManager::getAttendanceRepository() noexcept { return attendanceRepo_; }

} // namespace repo
