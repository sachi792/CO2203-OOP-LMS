#include "Administrator.h"
#include "Course.h"
#include "Student.h"
#include "Lecturer.h"
#include "LectureBasedCourse.h"
#include "LabBasedCourse.h"
#include "ProjectBasedCourse.h"
#include <sstream>
#include <vector>

namespace domain {
namespace {
std::vector<std::string> split(const std::string& text) {
    std::vector<std::string> out; std::stringstream ss(text); std::string x;
    while (std::getline(ss, x, '|')) out.push_back(x);
    return out;
}
}
Administrator::Administrator(std::string personId, std::string name, std::string email, std::string password)
    : Person(std::move(personId), std::move(name), std::move(email), std::move(password)) {}
Dashboard Administrator::getDashboard() const { return Dashboard{}; }

// details: STUDENT|personId|name|email|password|studentId
//          LECTURER|personId|name|email|password|department|staffId
//          ADMIN|personId|name|email|password
std::unique_ptr<Person> Administrator::createUser(const std::string& details) {
    auto f=split(details); if(f.empty()) return nullptr;
    if(f[0]=="STUDENT" && f.size()>=6) return std::make_unique<Student>(f[1],f[2],f[3],f[4],f[5]);
    if(f[0]=="LECTURER" && f.size()>=7) return std::make_unique<Lecturer>(f[1],f[2],f[3],f[4],f[5],f[6]);
    if(f[0]=="ADMIN" && f.size()>=5) return std::make_unique<Administrator>(f[1],f[2],f[3],f[4]);
    return nullptr;
}
void Administrator::editUser(Person& person) { (void)person; }
void Administrator::removeUser(const Person& person) { (void)person; }

// details Course:  LECTURE|code|title|credits|capacity
//                  PROJECT|code|title|credits|capacity
//              LAB|code|title|credits|capacity|labCapacity
std::unique_ptr<Course> Administrator::createCourse(const std::string& details) {
    auto f=split(details); if(f.size()<5) return nullptr;
    int credits=std::stoi(f[3]), capacity=std::stoi(f[4]);
    if(f[0]=="LECTURE") return std::make_unique<LectureBasedCourse>(f[1],f[2],credits,capacity);
    if(f[0]=="PROJECT") return std::make_unique<ProjectBasedCourse>(f[1],f[2],credits,capacity);
    if(f[0]=="LAB" && f.size()>=6) return std::make_unique<LabBasedCourse>(f[1],f[2],credits,capacity,std::stoi(f[5]));
    return nullptr;
}
void Administrator::editCourse(Course& course) { (void)course; }
void Administrator::removeCourse(const Course& course) { (void)course; }
} 
