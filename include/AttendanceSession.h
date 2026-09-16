#pragma once
#include <chrono>

namespace domain { class Course; }

namespace attendance {

// One lecturer-opened attendance window for a single Course.
class AttendanceSession {
public:
    AttendanceSession(int sessionId, domain::Course& course, int expiryMinutes);

    void open();
    void close();
    bool hasExpired() const;
    bool isOpen() const noexcept;

    int getSessionId() const noexcept;
    domain::Course& getCourse() const noexcept;

private:
    int sessionId_;
    domain::Course* course_; // non-owning
    bool isOpen_ = false;
    int expiryMinutes_;
    std::chrono::steady_clock::time_point openedAt_;
};

} // namespace attendance
