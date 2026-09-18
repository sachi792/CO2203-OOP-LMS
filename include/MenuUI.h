#pragma once
#include "Person.h"

namespace domain {
class Student;
class Lecturer;
class Administrator;
}

namespace repo {

// Console menu for the different user roles.
class MenuUI {
public:
    static void Menu(domain::Person& person);

private:
    static void showStudentMenu(domain::Student& student);
    static void showLecturerMenu(domain::Lecturer& lecturer);
    static void showAdminMenu(domain::Administrator& admin);
};

} // namespace repo
