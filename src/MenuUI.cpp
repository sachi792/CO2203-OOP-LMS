#include "MenuUI.h"
#include "Student.h"
#include "Lecturer.h"
#include "Administrator.h"
#include "SystemManager.h"
#include "Course.h"
#include "LMSException.h"
#include <iostream>

namespace repo {

// Reads one command line the same way Member 2's ConsoleCardReader does:
// a single std::getline() call. In an interactive terminal this waits for
// real input; in this project's automated/demo runs (stdin closed/empty),
// getline() hits EOF immediately and returns false, so the menu prints its
// options and then exits gracefully rather than hanging.
namespace {
bool readCommand(std::string& out) {
    return static_cast<bool>(std::getline(std::cin, out));
}
}

void MenuUI::Menu(domain::Person& person) {
    if (auto* student = dynamic_cast<domain::Student*>(&person)) {
        showStudentMenu(*student);
    } else if (auto* lecturer = dynamic_cast<domain::Lecturer*>(&person)) {
        showLecturerMenu(*lecturer);
    } else if (auto* admin = dynamic_cast<domain::Administrator*>(&person)) {
        showAdminMenu(*admin);
    } else {
        std::cout << "[MenuUI] Unrecognised Person subtype - no menu available\n";
    }
}

void MenuUI::showStudentMenu(domain::Student& student) {
    while (true) {
        std::cout << "\n=== Student Menu: " << student.getName() << " ===\n"
                  << "1) Register for a course\n"
                  << "2) Drop a course\n"
                  << "3) View timetable\n"
                  << "4) View attendance\n"
                  << "0) Logout\n"
                  << "Enter a choice: ";

        std::string choice;
        if (!readCommand(choice)) return;

        try {
            if (choice == "1") {
                std::cout << "Available courses:\n";
                const auto& courses = SystemManager::getInstance().getCourseRepository().getItems();
                for (const auto& course : courses) {
                    if (course) {
                        std::cout << "  " << course->getCode() << " - " << course->getTitle() << "\n";
                    }
                }

                std::cout << "Enter course code: ";
                std::string code;
                if (!readCommand(code)) return;

                auto course = SystemManager::getInstance().getCourseRepository().findByCode(code);
                if (!course) {
                    std::cout << "Course not found.\n";
                    continue;
                }

                // Avoid attempting to enrol in the same course twice.
                // Without this guard, the existing timetable slot is detected as
                // a clash with itself, which gives the user a misleading error.
                if (student.isEnrolledIn(code)) {
                    std::cout << "Already enrolled in " << code << ".\n";
                    continue;
                }

                student.registerCourse(*course);
                std::cout << "Registered for " << code << ".\n";
            } else if (choice == "2") {
                std::cout << "Enter course code to drop: ";
                std::string code;
                if (!readCommand(code)) return;
                student.dropCourse(code);
                std::cout << "Dropped " << code << ".\n";
            } else if (choice == "3") {
                student.viewTimetable();
            } else if (choice == "4") {
                student.viewAttendance();
            } else if (choice == "0") {
                return;
            } else {
                std::cout << "Invalid selection.\n";
            }
        } catch (const domain::LMSException& ex) {
            std::cout << "Operation error: " << ex.what() << "\n";
        } catch (const std::exception& ex) {
            std::cout << "Operation error: " << ex.what() << "\n";
        }
    }
}

void MenuUI::showLecturerMenu(domain::Lecturer& lecturer) {
    while (true) {
        std::cout << "\n=== Lecturer Menu: " << lecturer.getName() << " ===\n"
                  << "1) Open attendance session\n"
                  << "2) Mark attendance\n"
                  << "3) View courses\n"
                  << "4) Select capture method\n"
                  << "5) Close attendance session\n"
                  << "6) View attendance report\n"
                  << "7) Correct attendance\n"
                  << "0) Logout\n"
                  << "Enter a choice: ";

        std::string choice;
        if (!readCommand(choice)) return;

        if (choice == "1") lecturer.openAttendance();
        else if (choice == "2") lecturer.markAttendance();
        else if (choice == "3") lecturer.viewCourses();
        else if (choice == "4") lecturer.selectCaptureMethod();
        else if (choice == "5") lecturer.closeAttendance();
        else if (choice == "6") lecturer.viewAttendanceReport();
        else if (choice == "7") lecturer.correctAttendance();
        else if (choice == "0") return;
        else std::cout << "Invalid selection.\n";
    }
}

void MenuUI::showAdminMenu(domain::Administrator& admin) {
    (void)admin;
    std::cout << "\n=== Administrator Menu ===\n"
              << "1) Create user\n"
              << "2) Edit user\n"
              << "3) Remove user\n"
              << "4) Create course\n"
              << "5) Edit course\n"
              << "6) Remove course\n"
              << "Enter a choice (or press Ctrl+D to skip in this demo): ";
    std::string choice;
    if (!readCommand(choice)) {
        std::cout << "\n[No input received - non-interactive demo, skipping interactive loop]\n";
        return;
    }
    std::cout << "(Selection " << choice << " would dispatch to the matching Administrator method)\n";
}

} // namespace repo
