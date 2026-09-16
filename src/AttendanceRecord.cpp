#include "AttendanceRecord.h"
#include "AttendanceSession.h"
#include "Student.h" // domain::Student - needed here (not just forward-declared) for display()
#include <iostream>
#include <ctime>

namespace attendance {

AttendanceRecord::AttendanceRecord(domain::Student* student, AttendanceSession* session,
                                    std::string status, std::string captureMethod)
    : student_(student), session_(session), status_(std::move(status)),
      timestamp_(std::time(nullptr)), captureMethod_(std::move(captureMethod)) {}

void AttendanceRecord::display() const {
    std::cout << "[Session " << session_->getSessionId() << "] "
              << student_->getStudentId() << " - " << status_
              << " (" << captureMethod_ << ")\n";
}

domain::Student* AttendanceRecord::getStudent() const noexcept { return student_; }
AttendanceSession* AttendanceRecord::getSession() const noexcept { return session_; }
const std::string& AttendanceRecord::getStatus() const noexcept { return status_; }
std::time_t AttendanceRecord::getTimestamp() const noexcept { return timestamp_; }
const std::string& AttendanceRecord::getCaptureMethod() const noexcept { return captureMethod_; }

} // namespace attendance
