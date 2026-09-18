#include "SystemManager.h"
#include "Student.h"
#include "Lecturer.h"
#include "Course.h"
#include "Enrollment.h"
#include "LMSException.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace repo {
SystemManager::SystemManager() {
    userRepo_.loadAll();
    courseRepo_.loadAll();
    attendanceRepo_.loadAll();
    loadRelations();
}
SystemManager& SystemManager::getInstance() { static SystemManager instance; return instance; }
bool SystemManager::login(const std::string& email, const std::string& password) {
    auto person=userRepo_.findByEmail(email);
    if(person && person->login(email,password)){ currentUser_=person.get(); return true; }
    return false;
}
void SystemManager::logout(){ currentUser_=nullptr; }
domain::Person* SystemManager::getCurrentUser() const noexcept { return currentUser_; }
UserRepository& SystemManager::getUserRepository() noexcept { return userRepo_; }
CourseRepository& SystemManager::getCourseRepository() noexcept { return courseRepo_; }
AttendanceRepository& SystemManager::getAttendanceRepository() noexcept { return attendanceRepo_; }

void SystemManager::saveAll() {
    userRepo_.saveAll(); courseRepo_.saveAll(); attendanceRepo_.saveAll();
    std::ofstream out("relations.dat", std::ios::trunc);
    for(const auto& c: courseRepo_.getItems()) {
        if(!c) continue;
        if(c->getLecturer()) out << "LECTURER|" << c->getCode() << "|" << c->getLecturer()->getPersonId() << "\n";
        for(auto* pre: c->getPrerequisites()) if(pre) out << "PREREQ|" << c->getCode() << "|" << pre->getCode() << "\n";
    }
    for(const auto& p: userRepo_.getItems()) {
        auto st=std::dynamic_pointer_cast<domain::Student>(p); if(!st) continue;
        for(const auto& e: st->getEnrollments()) if(e && e->isActive())
            out << "ENROLL|" << st->getPersonId() << "|" << e->getCourse()->getCode() << "\n";
    }
}

void SystemManager::loadRelations() {
    std::ifstream in("relations.dat"); if(!in) return;
    std::vector<std::string> enrollLines; std::string line;
    while(std::getline(in,line)) {
        std::stringstream ss(line); std::string type,a,b;
        std::getline(ss,type,'|'); std::getline(ss,a,'|'); std::getline(ss,b,'|');
        if(type=="PREREQ") { auto c=courseRepo_.findByCode(a), p=courseRepo_.findByCode(b); if(c&&p) c->addPrerequisite(p.get()); }
        else if(type=="LECTURER") { auto c=courseRepo_.findByCode(a); auto u=userRepo_.findById(b); auto l=std::dynamic_pointer_cast<domain::Lecturer>(u); if(c&&l){ c->setLecturer(l.get()); l->assignCourse(c.get()); } }
        else if(type=="ENROLL") enrollLines.push_back(line);
    }
    for(const auto& x: enrollLines) {
        std::stringstream ss(x); std::string type,pid,code; std::getline(ss,type,'|'); std::getline(ss,pid,'|'); std::getline(ss,code,'|');
        auto u=userRepo_.findById(pid); auto st=std::dynamic_pointer_cast<domain::Student>(u); auto c=courseRepo_.findByCode(code);
        if(st&&c&&!st->isEnrolledIn(code)) { try { st->registerCourse(*c); } catch(const std::exception& ex) { std::cerr << "[relations] " << ex.what() << "\n"; } }
    }
}
}
