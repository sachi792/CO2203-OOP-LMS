#pragma once
#include "Person.h"
#include <memory>

namespace domain {

class Course;

// Note: createUser/createCourse return owning results (unique_ptr) since the
// Administrator FACTORY-creates them; ownership then passes to whichever
// repository (Member 3) persists them.
class Administrator : public Person {
public:
    Administrator(std::string personId, std::string name, std::string email, std::string password);

    Dashboard getDashboard() const override;

    std::unique_ptr<Person> createUser(const std::string& details);
    void editUser(Person& person);
    void removeUser(const Person& person);

    std::unique_ptr<Course> createCourse(const std::string& details);
    void editCourse(Course& course);
    void removeCourse(const Course& course);
};

} // namespace domain
