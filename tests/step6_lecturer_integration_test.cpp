#include <iostream>
#include <sstream>
#include "Lecturer.h"
#include "Student.h"
#include "LectureBasedCourse.h"

int main() {
    domain::Lecturer lecturer("P10", "Dr Silva", "silva@uni.lk", "pass", "Computing", "LEC01");
    domain::Student student("P20", "Alice", "alice@uni.lk", "pass", "ST001");
    domain::LectureBasedCourse course("CO2203", "OOP", 3, 30);
    course.setSchedule({"Monday", 9, 11, "Lab A"});
    lecturer.assignCourse(&course);
    student.registerCourse(course);

    std::istringstream fakeInput("CO2203\nST001\nST001\n1\n");
    auto* old = std::cin.rdbuf(fakeInput.rdbuf());

    lecturer.openAttendance();
    lecturer.markAttendance();
    lecturer.markAttendance(); // duplicate should be rejected by AttendanceRegister
    lecturer.selectCaptureMethod();

    std::cin.rdbuf(old);
    return 0;
}
