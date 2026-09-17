#pragma once
#include <vector>
#include <string>
#include "AttendanceRecord.h"

namespace domain { class Student; class Course; }

namespace attendance {

class AttendanceSession;

class AttendanceRegister {
public:
    // Throws:
    //   - attendance::SessionClosedException   if the session isn't open / has expired
    //   - domain::NotEnrolledException         if the student isn't enrolled in the course
    //                                           (reuses Member 1's exception - see README)
    //   - attendance::DuplicateAttendanceException if already marked for this session
    void markAttendance(domain::Student& student, AttendanceSession& session,
                         const std::string& captureMethod);

    // Append-only correction: the original attendance record is never edited
    // or deleted. A new record is appended with the corrected status and an
    // audit description in captureMethod.
    void correctAttendance(domain::Student& student, AttendanceSession& session,
                           const std::string& correctedStatus,
                           const std::string& reason);

    // Naive percentage: (Present records for this student+course) /
    // (distinct sessions seen for this course in the register) * 100.
    // See README for the limitation this simplification introduces.
    double attendancePercentage(const domain::Student& student, const domain::Course& course) const;

    void report() const;

    const std::vector<AttendanceRecord>& getRecords() const noexcept;

private:
    std::vector<AttendanceRecord> records_;

    bool hasExistingRecord(const domain::Student& student, const AttendanceSession& session) const;
};

} // namespace attendance
