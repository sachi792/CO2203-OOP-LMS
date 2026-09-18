#pragma once
#include <string>
#include <ctime>

namespace domain { class Student; }

namespace attendance {

class AttendanceSession;

class AttendanceRecord {
public:
    AttendanceRecord(domain::Student* student, AttendanceSession* session,
                      std::string status, std::string captureMethod);

    void display() const;

    domain::Student* getStudent() const noexcept;
    AttendanceSession* getSession() const noexcept;
    const std::string& getStatus() const noexcept;
    std::time_t getTimestamp() const noexcept;
    const std::string& getCaptureMethod() const noexcept;

private:
    domain::Student* student_;  
    AttendanceSession* session_; 
    std::string status_;
    std::time_t timestamp_;
    std::string captureMethod_;
};

} 
