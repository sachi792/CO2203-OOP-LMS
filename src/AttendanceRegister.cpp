#include "AttendanceRegister.h"
#include "AttendanceSession.h"
#include "AttendanceExceptions.h"
#include "Student.h"
#include "Course.h"
#include "NotEnrolledException.h"
#include <iostream>
#include <set>

namespace attendance {

void AttendanceRegister::markAttendance(domain::Student& student, AttendanceSession& session,
                                         const std::string& captureMethod) {
    if (!session.isOpen() || session.hasExpired()) {
        throw SessionClosedException(session.getSessionId());
    }

    if (!student.isEnrolledIn(session.getCourse().getCode())) {
        // Reuses Member 1's exception on purpose: "not enrolled" is a
        // domain-level fact, not an attendance-specific error condition.
        throw domain::NotEnrolledException(student.getStudentId(), session.getCourse().getCode());
    }

    if (hasExistingRecord(student, session)) {
        throw DuplicateAttendanceException(student.getStudentId(), session.getSessionId());
    }

    records_.emplace_back(&student, &session, "Present", captureMethod);
}

double AttendanceRegister::attendancePercentage(const domain::Student& student,
                                                 const domain::Course& course) const {
    int presentCount = 0;
    std::set<int> distinctSessionIds;

    for (const auto& record : records_) {
        if (record.getSession()->getCourse().getCode() != course.getCode()) continue;
        distinctSessionIds.insert(record.getSession()->getSessionId());
        if (record.getStudent() == &student && record.getStatus() == "Present") {
            ++presentCount;
        }
    }

    if (distinctSessionIds.empty()) return 0.0;
    return (static_cast<double>(presentCount) / static_cast<double>(distinctSessionIds.size())) * 100.0;
}

void AttendanceRegister::report() const {
    std::cout << "--- Attendance Register (" << records_.size() << " record(s)) ---\n";
    for (const auto& record : records_) {
        record.display();
    }
}

const std::vector<AttendanceRecord>& AttendanceRegister::getRecords() const noexcept {
    return records_;
}

bool AttendanceRegister::hasExistingRecord(const domain::Student& student,
                                           const AttendanceSession& session) const {
    for (const auto& record : records_) {
        if (record.getStudent() == &student && record.getSession() == &session) {
            return true;
        }
    }
    return false;
}

} // namespace attendance
