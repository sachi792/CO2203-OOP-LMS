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
    if (!sys.getUserRepository().getItems().empty() || !sys.getCourseRepository().getItems().empty()) return;
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

    try {
        alice->registerCourse(*oop);
        std::cout << "[seed] Alice registered for CO2203\n";
    } catch (const domain::LMSException& ex) {
        std::cout << "[seed] Unexpected enrolment failure: " << ex.what() << "\n";
    }
    sys.saveAll();
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

    // Keep the application alive across logouts. This also lets a lecturer
    // mark attendance and then a student log in during the same run.
    while (true) {
        std::cout << "\nEmail (or 0 to exit): ";
        std::string email;
        if (!std::getline(std::cin, email)) {
            std::cout << "\n[No input received - exiting.]\n";
            break;
        }
        if (email == "0") break;

        std::cout << "Password: ";
        std::string password;
        if (!std::getline(std::cin, password)) break;

        if (!sys.login(email, password)) {
            std::cout << "Login failed. Try again.\n";
            continue;
        }

        std::cout << "Logged in as " << sys.getCurrentUser()->getName() << "\n";
        MenuUI::Menu(*sys.getCurrentUser());

        sys.logout();
        std::cout << "Logged out.\n";
    }

    // Keep repository data flushed on a normal exit too.
    sys.saveAll();
    std::cout << "Goodbye.\n";
    return 0;
}
