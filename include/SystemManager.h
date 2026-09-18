#pragma once 
#include "UserRepository.h" 
#include "CourseRepository.h" 
#include "AttendanceRepository.h" 
 
namespace repo { 
 
// Keeps one SystemManager instance and manages the repositories and logged-in user.
class SystemManager { 
public: 
    static SystemManager& getInstance(); 
 
    SystemManager(const SystemManager&) = delete; 
    SystemManager& operator=(const SystemManager&) = delete; 
 
    // Checks the login details and sets the current user.
    bool login(const std::string& email, const std::string& password); 
    void logout(); 
 
    domain::Person* getCurrentUser() const noexcept; 
 
    UserRepository& getUserRepository() noexcept; 
    CourseRepository& getCourseRepository() noexcept; 
    AttendanceRepository& getAttendanceRepository() noexcept; 
    void saveAll(); 
    void loadRelations(); 
 
private: 
    SystemManager(); 
 
    UserRepository userRepo_; 
    CourseRepository courseRepo_; 
    AttendanceRepository attendanceRepo_; 
    domain::Person* currentUser_ = nullptr;  
}; 
 
}