#pragma once
#include "Person.h"

namespace domain {
class Student;
class Lecturer;
class Administrator;
}

namespace repo {

// Console-based menu, dispatching to the person's own operations based on
// their concrete role. Matches the UML's "MenuUI + Menu(Person): void".
class MenuUI {
public:
    static void Menu(domain::Person& person);

private:
    static void showStudentMenu(domain::Student& student);
    static void showLecturerMenu(domain::Lecturer& lecturer);
    static void showAdminMenu(domain::Administrator& admin);
};

} // namespace repo
