#include "MenuUI.h"
#include "Student.h"
#include "Lecturer.h"
#include "Administrator.h"
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
    std::cout << "\n=== Student Menu: " << student.getName() << " ===\n"
              << "1) Register for a course\n"
              << "2) Drop a course\n"
              << "3) View timetable\n"
              << "4) View attendance\n"
              << "Enter a choice (or press Ctrl+D to skip in this demo): ";
    std::string choice;
    if (!readCommand(choice)) {
        std::cout << "\n[No input received - non-interactive demo, skipping interactive loop]\n";
        return;
    }
    if (choice == "3") student.viewTimetable();
    else if (choice == "4") student.viewAttendance();
    else std::cout << "(Selection " << choice << " would dispatch to the matching Student method)\n";
}

void MenuUI::showLecturerMenu(domain::Lecturer& lecturer) {
    std::cout << "\n=== Lecturer Menu: " << lecturer.getName() << " ===\n"
              << "1) Open attendance session\n"
              << "2) Mark attendance\n"
              << "3) View courses\n"
              << "4) Select capture method\n"
              << "Enter a choice (or press Ctrl+D to skip in this demo): ";
    std::string choice;
    if (!readCommand(choice)) {
        std::cout << "\n[No input received - non-interactive demo, skipping interactive loop]\n";
        return;
    }
    if (choice == "3") lecturer.viewCourses();
    else std::cout << "(Selection " << choice << " would dispatch to the matching Lecturer method)\n";
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
