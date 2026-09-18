#include "Student.h"
#include "LectureBasedCourse.h"
#include "AttendanceSession.h"
#include "AttendanceRegister.h"
#include <iostream>
#include <iomanip>

int main() {
    domain::Student s1("P1", "Student One", "s1@uni.lk", "pw", "ST001");
    domain::Student s2("P2", "Student Two", "s2@uni.lk", "pw", "ST002");
    domain::LectureBasedCourse course("CO2203", "OOP", 3, 30);

    s1.registerCourse(course);
    s2.registerCourse(course);

    attendance::AttendanceRegister reg;

    attendance::AttendanceSession session1(1, course, 5);
    session1.open();
    reg.markAttendance(s1, session1, "CardTap");
    reg.markAttendance(s2, session1, "CardTap");
    session1.close();

    attendance::AttendanceSession session2(2, course, 5);
    session2.open();
    reg.markAttendance(s2, session2, "CardTap");
    session2.close();

    std::cout << std::fixed << std::setprecision(0);
    std::cout << "ST001 percentage: " << reg.attendancePercentage(s1, course) << "%\n";
    std::cout << "ST002 percentage: " << reg.attendancePercentage(s2, course) << "%\n";
    reg.report();

    return 0;
}
