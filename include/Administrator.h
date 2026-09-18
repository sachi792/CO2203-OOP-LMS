#pragma once
#include "Person.h"
#include <memory>

namespace domain {

class Course;

 // createUser/createCourse return unique_ptr because the Administrator
 // creates them first, then the repository takes ownership and stores them.
 
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

} 
