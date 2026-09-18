#pragma once
#include <vector>
#include <string>
#include "AttendanceRecord.h"

namespace domain { class Student; class Course; }

namespace attendance {

class AttendanceSession;

class AttendanceRegister {
public:
     // Checks the session, enrollment and duplicate records before marking attendance.
    void markAttendance(domain::Student& student, AttendanceSession& session,
                         const std::string& captureMethod);

    // Keep the old record and add a new one with the corrected status and reason.
    void correctAttendance(domain::Student& student, AttendanceSession& session,
                           const std::string& correctedStatus,
                           const std::string& reason);

    // Calculates the percentage using the attendance records in the register.
    double attendancePercentage(const domain::Student& student, const domain::Course& course) const;

    void report() const;

    const std::vector<AttendanceRecord>& getRecords() const noexcept;

private:
    std::vector<AttendanceRecord> records_;

    bool hasExistingRecord(const domain::Student& student, const AttendanceSession& session) const;
};

}
