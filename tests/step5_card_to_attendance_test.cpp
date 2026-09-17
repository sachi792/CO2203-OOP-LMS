#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "Student.h"
#include "LectureBasedCourse.h"
#include "StudentCard.h"
#include "CardTapCapture.h"
#include "AttendanceSession.h"
#include "AttendanceRegister.h"
#include "LMSException.h"

// Step 5 integration test only.
// No new production class is introduced: it connects the existing UML classes
// exactly as the application/menu layer is expected to do.
int main() {
    domain::Student student("P200", "Alice", "alice@uni.lk", "pass", "ST001");
    domain::LectureBasedCourse course("CO2203", "OOP", 3, 30);
    course.setSchedule({"Monday", 9, 11, "Room A"});
    student.registerCourse(course);

    attendance::StudentCard card("CARD-ST001", &student);
    std::vector<attendance::StudentCard*> cards{&card};

    attendance::AttendanceSession session(1, course, 5);
    attendance::AttendanceRegister attendanceRegister;
    attendance::CardTapCapture capture;

    session.open();
    capture.beginSession();

    auto uid = capture.captureNext();
    if (!uid) {
        capture.endSession();
        session.close();
        std::cout << "No card captured\n";
        return 0;
    }

    domain::Student* resolvedStudent = nullptr;
    for (auto* knownCard : cards) {
        if (knownCard != nullptr && knownCard->getUID() == *uid) {
            resolvedStudent = knownCard->getOwner();
            break;
        }
    }

    if (resolvedStudent == nullptr) {
        std::cout << "Unknown card UID: " << *uid << "\n";
    } else {
        try {
            attendanceRegister.markAttendance(*resolvedStudent, session, "CardTap");
            std::cout << "Attendance marked for " << resolvedStudent->getStudentId() << "\n";
        } catch (const domain::LMSException& ex) {
            std::cout << "Attendance error: " << ex.what() << "\n";
        }
    }

    capture.endSession();
    session.close();
    attendanceRegister.report();
    return 0;
}
