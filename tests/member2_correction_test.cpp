#include <iostream>
#include "Student.h"
#include "LectureBasedCourse.h"
#include "AttendanceSession.h"
#include "AttendanceRegister.h"
#include "AttendanceRepository.h"
#include <cstdio>

int main() {
    domain::Student student("P1", "Alice", "alice@test", "pw", "ST001");
    domain::LectureBasedCourse course("CO2203", "OOP", 3, 30);
    student.registerCourse(course);

    attendance::AttendanceSession session(1, course, 5);
    session.open();

    attendance::AttendanceRegister reg;
    reg.markAttendance(student, session, "Lecturer");
    reg.correctAttendance(student, session, "Absent", "Marked by mistake");

    const auto& records = reg.getRecords();
    std::cout << "records=" << records.size() << "\n";
    std::cout << "original=" << records[0].getStatus() << "\n";
    std::cout << "correction=" << records[1].getStatus() << "\n";
    std::cout << "audit=" << records[1].getCaptureMethod() << "\n";
    std::cout << "percentage=" << reg.attendancePercentage(student, course) << "\n";

    const char* path = "/tmp/member2_correction_attendance.dat";
    std::remove(path);
    repo::AttendanceRepository repo(path);
    repo.addFromDomainRecord(records[0]);
    repo.addFromDomainRecord(records[1]);

    repo::AttendanceRepository loaded(path);
    loaded.loadAll();
    std::cout << "persisted=" << loaded.getItems().size() << "\n";
    std::cout << "persisted_original=" << loaded.getItems()[0].status << "\n";
    std::cout << "persisted_correction=" << loaded.getItems()[1].status << "\n";
    std::cout << "persisted_audit=" << loaded.getItems()[1].captureMethod << "\n";
    std::remove(path);
    return 0;
}
