// CO2203 LMS - single entry point tying all three members' modules together.
//
// This seeds a small amount of demo data (so the program is runnable and
// demonstrable without a separate admin-driven "first run" setup step -
// see Administrator::createUser()'s open question in the domain README)
// and then runs a real interactive loop: log in, see your role-specific
// menu, log out, or exit. Run it in a real terminal for the interactive
// experience; run it with input piped in (or redirected from /dev/null)
// for scripted/non-interactive use - see MenuUI's own comment on why that
// doesn't hang.
#include <iostream>
#include <memory>

#include "SystemManager.h"
#include "MenuUI.h"

#include "Student.h"
#include "Lecturer.h"
#include "Administrator.h"
#include "LectureBasedCourse.h"
#include "Timetable.h"

#include "LMSExceptions.h"

using namespace repo;

namespace {

void seedDemoData(SystemManager& sys) {
    auto drSmith = std::make_shared<domain::Lecturer>("P100", "Dr. Smith", "smith@uni.lk", "pass123",
                                                        "Computing", "S001");
    auto alice = std::make_shared<domain::Student>("P200", "Alice", "alice@uni.lk", "pass456", "ST001");
    auto admin = std::make_shared<domain::Administrator>("P900", "Root Admin", "admin@uni.lk", "adminpw");

    sys.getUserRepository().add(drSmith);
    sys.getUserRepository().add(alice);
    sys.getUserRepository().add(admin);

    auto oop = std::make_shared<domain::LectureBasedCourse>("CO2203", "Object Oriented Programming", 3, 30);
    oop->setSchedule({"Monday", 9, 11, "Room A"});
    sys.getCourseRepository().add(oop);
    drSmith->assignCourse(oop.get());

    // Give Alice a Timetable so Enrollment's clash check has something to
    // compare against (see Course.h / Enrollment.cpp for why this exists).
    // Student only holds a non-owning pointer (see Student.h), so this needs
    // to outlive the Student - a static local does that simply for a
    // single-run demo program; a real system would have SystemManager or a
    // repository own Timetable instances properly.
    static attendance::Timetable aliceTimetable;
    alice->setTimetable(&aliceTimetable);

    try {
        alice->registerCourse(*oop);
        std::cout << "[seed] Alice registered for CO2203\n";
    } catch (const domain::LMSException& ex) {
        std::cout << "[seed] Unexpected enrolment failure: " << ex.what() << "\n";
    }
}

void printWelcome() {
    std::cout << "=========================================\n"
              << " CO2203 Learning Management System\n"
              << "=========================================\n"
              << "Seeded accounts (email / password):\n"
              << "  smith@uni.lk / pass123   (Lecturer)\n"
              << "  alice@uni.lk / pass456   (Student)\n"
              << "  admin@uni.lk / adminpw   (Administrator)\n"
              << "-----------------------------------------\n";
}

} // namespace

int main() {
    auto& sys = SystemManager::getInstance();
    seedDemoData(sys);
    printWelcome();

    std::cout << "Email: ";
    std::string email;
    if (!std::getline(std::cin, email)) {
        std::cout << "\n[No input received - exiting. Run interactively to log in.]\n";
        return 0;
    }

    std::cout << "Password: ";
    std::string password;
    std::getline(std::cin, password);

    if (!sys.login(email, password)) {
        std::cout << "Login failed.\n";
        return 1;
    }

    std::cout << "Logged in as " << sys.getCurrentUser()->getName() << "\n";
    MenuUI::Menu(*sys.getCurrentUser());

    sys.logout();
    std::cout << "Logged out.\n";
    return 0;
}
