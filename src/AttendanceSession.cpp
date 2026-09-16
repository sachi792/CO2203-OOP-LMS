#include "AttendanceSession.h"

namespace attendance {

AttendanceSession::AttendanceSession(int sessionId, domain::Course& course, int expiryMinutes)
    : sessionId_(sessionId), course_(&course), expiryMinutes_(expiryMinutes) {}

void AttendanceSession::open() {
    isOpen_ = true;
    openedAt_ = std::chrono::steady_clock::now();
}

void AttendanceSession::close() {
    isOpen_ = false;
}

bool AttendanceSession::hasExpired() const {
    if (!isOpen_) return true;
    auto elapsed = std::chrono::steady_clock::now() - openedAt_;
    return elapsed > std::chrono::minutes(expiryMinutes_);
}

bool AttendanceSession::isOpen() const noexcept {
    return isOpen_;
}

int AttendanceSession::getSessionId() const noexcept { return sessionId_; }
domain::Course& AttendanceSession::getCourse() const noexcept { return *course_; }

} // namespace attendance
