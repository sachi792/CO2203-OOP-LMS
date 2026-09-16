#include "Administrator.h"
#include "Course.h"
#include <iostream>

namespace domain {

Administrator::Administrator(std::string personId, std::string name, std::string email,
                              std::string password)
    : Person(std::move(personId), std::move(name), std::move(email), std::move(password)) {}

Dashboard Administrator::getDashboard() const {
    return Dashboard{};
}

std::unique_ptr<Person> Administrator::createUser(const std::string& details) {
    // TODO(integration): "details" needs an agreed format (e.g. CSV/JSON)
    // once Member 3's MenuUI decides how it collects admin input, and this
    // needs to decide WHICH Person subclass to construct (Student/Lecturer).
    // Left as a stub for now - not something Member 1 can finalise alone.
    std::cout << "[Administrator] createUser(" << details << ") - awaiting agreed input format\n";
    return nullptr;
}

void Administrator::editUser(Person& person) {
    std::cout << "[Administrator] editUser(" << person.getPersonId() << ")\n";
}

void Administrator::removeUser(const Person& person) {
    std::cout << "[Administrator] removeUser(" << person.getPersonId() << ")\n";
}

std::unique_ptr<Course> Administrator::createCourse(const std::string& details) {
    // Same integration note as createUser(): also needs to pick which
    // concrete Course subclass (Lecture/Lab/Project) to instantiate.
    std::cout << "[Administrator] createCourse(" << details << ") - awaiting agreed input format\n";
    return nullptr;
}

void Administrator::editCourse(Course& course) {
    std::cout << "[Administrator] editCourse(" << course.getCode() << ")\n";
}

void Administrator::removeCourse(const Course& course) {
    std::cout << "[Administrator] removeCourse(" << course.getCode() << ")\n";
}

} // namespace domain
