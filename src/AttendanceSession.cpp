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
    // A session that was never opened or was manually closed is not
    // considered "expired". Expiry only applies to an active window.
    if (!isOpen_) return false;

    const auto elapsed = std::chrono::steady_clock::now() - openedAt_;
    return elapsed >= std::chrono::minutes(expiryMinutes_);
}

bool AttendanceSession::isOpen() const noexcept {
    // Once the configured time window expires, the session is no longer
    // logically open even if close() was not called manually.
    return isOpen_ && !hasExpired();
}

int AttendanceSession::getSessionId() const noexcept { return sessionId_; }
domain::Course& AttendanceSession::getCourse() const noexcept { return *course_; }

} // namespace attendance
