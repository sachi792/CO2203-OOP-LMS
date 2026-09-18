#include <iostream>
#include <sstream>
#include <memory>
#include "SystemManager.h"
#include "Student.h"
#include "Lecturer.h"
#include "LectureBasedCourse.h"

int main() {
    auto student = std::make_shared<domain::Student>("P1", "Alice", "a@u.lk", "pw", "ST001");
    auto lecturer = std::make_shared<domain::Lecturer>("P2", "Dr Smith", "l@u.lk", "pw", "CS", "L1");
    auto course = std::make_shared<domain::LectureBasedCourse>("CO2203", "OOP", 3, 30);
    student->registerCourse(*course);
    lecturer->assignCourse(course.get());

    // Open CO2203, then manually mark ST001.
    std::istringstream fakeInput("CO2203\nST001\n");
    auto* old = std::cin.rdbuf(fakeInput.rdbuf());
    lecturer->openAttendance();
    lecturer->markAttendance();
    std::cin.rdbuf(old);

    student->viewAttendance();
    return 0;
}
