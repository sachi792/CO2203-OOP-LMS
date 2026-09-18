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
    // Only an open session can expire.
    if (!isOpen_) return false;

    const auto elapsed = std::chrono::steady_clock::now() - openedAt_;
    return elapsed >= std::chrono::minutes(expiryMinutes_);
}

bool AttendanceSession::isOpen() const noexcept {
    // Treat the session as closed once its time limit is reached.
    return isOpen_ && !hasExpired();
}

int AttendanceSession::getSessionId() const noexcept { return sessionId_; }
domain::Course& AttendanceSession::getCourse() const noexcept { return *course_; }

} 
