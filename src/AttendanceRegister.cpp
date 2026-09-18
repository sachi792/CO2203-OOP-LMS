#include "AttendanceRegister.h"
#include "AttendanceSession.h"
#include "AttendanceExceptions.h"
#include "Student.h"
#include "Course.h"
#include "NotEnrolledException.h"
#include <iostream>
#include <set>
#include <map>
#include <stdexcept>

namespace attendance {

void AttendanceRegister::markAttendance(domain::Student& student, AttendanceSession& session,
                                         const std::string& captureMethod) {
    if (!session.isOpen() || session.hasExpired()) {
        throw SessionClosedException(session.getSessionId());
    }

    if (!student.isEnrolledIn(session.getCourse().getCode())) {
        // Student must be enrolled in the course before attendance is marked..
        throw domain::NotEnrolledException(student.getStudentId(), session.getCourse().getCode());
    }

    if (hasExistingRecord(student, session)) {
        throw DuplicateAttendanceException(student.getStudentId(), session.getSessionId());
    }

    records_.emplace_back(&student, &session, "Present", captureMethod);
}

void AttendanceRegister::correctAttendance(domain::Student& student, AttendanceSession& session,
                                            const std::string& correctedStatus,
                                            const std::string& reason) {
    if (!hasExistingRecord(student, session)) {
        throw std::runtime_error("Cannot correct attendance: original record not found");
    }

    // Add a new record for the correction instead of changing the old one.
    const std::string auditMethod = reason.empty()
        ? "Correction"
        : "Correction: " + reason;
    records_.emplace_back(&student, &session, correctedStatus, auditMethod);
}

double AttendanceRegister::attendancePercentage(const domain::Student& student,
                                                 const domain::Course& course) const {
    std::set<int> distinctSessionIds;
    // Append-only corrections mean the latest record for a student/session is
    // the effective status, while earlier records remain available for audit.
    std::map<int, std::string> latestStatusBySession;

    for (const auto& record : records_) {
        if (record.getSession()->getCourse().getCode() != course.getCode()) continue;

        const int sessionId = record.getSession()->getSessionId();
        distinctSessionIds.insert(sessionId);
        if (record.getStudent() == &student) {
            latestStatusBySession[sessionId] = record.getStatus();
        }
    }

    if (distinctSessionIds.empty()) return 0.0;

    int presentCount = 0;
    for (const auto& entry : latestStatusBySession) {
        if (entry.second == "Present") ++presentCount;
    }

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
        if (record.getStudent() == &student &&
            record.getSession()->getSessionId() == session.getSessionId()) {
            return true;
        }
    }
    return false;
}

}
