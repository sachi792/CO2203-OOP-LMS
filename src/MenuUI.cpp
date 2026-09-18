#include "MenuUI.h"
#include "Student.h"
#include "Lecturer.h"
#include "Administrator.h"
#include "SystemManager.h"
#include "Course.h"
#include "Student.h"
#include "Lecturer.h"
#include "Enrollment.h"
#include "LMSException.h"
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
    while (true) {
        std::cout << "\n=== Student Menu: " << student.getName() << " ===\n"
                  << "1) Register for a course\n"
                  << "2) Drop a course\n"
                  << "3) View timetable\n"
                  << "4) View attendance\n"
                  << "0) Logout\n"
                  << "Enter a choice: ";

        std::string choice;
        if (!readCommand(choice)) return;

        try {
            if (choice == "1") {
                std::cout << "Available courses:\n";
                const auto& courses = SystemManager::getInstance().getCourseRepository().getItems();
                for (const auto& course : courses) {
                    if (course) {
                        std::cout << "  " << course->getCode() << " - " << course->getTitle() << "\n";
                    }
                }

                std::cout << "Enter course code: ";
                std::string code;
                if (!readCommand(code)) return;

                auto course = SystemManager::getInstance().getCourseRepository().findByCode(code);
                if (!course) {
                    std::cout << "Course not found.\n";
                    continue;
                }

                // Avoid attempting to enrol in the same course twice.
                // Without this guard, the existing timetable slot is detected as
                // a clash with itself, which gives the user a misleading error.
                if (student.isEnrolledIn(code)) {
                    std::cout << "Already enrolled in " << code << ".\n";
                    continue;
                }

                student.registerCourse(*course);
                SystemManager::getInstance().saveAll();
                std::cout << "Registered for " << code << ".\n";
            } else if (choice == "2") {
                std::cout << "Enter course code to drop: ";
                std::string code;
                if (!readCommand(code)) return;
                student.dropCourse(code);
                SystemManager::getInstance().saveAll();
                std::cout << "Dropped " << code << ".\n";
            } else if (choice == "3") {
                student.viewTimetable();
            } else if (choice == "4") {
                student.viewAttendance();
            } else if (choice == "0") {
                return;
            } else {
                std::cout << "Invalid selection.\n";
            }
        } catch (const domain::LMSException& ex) {
            std::cout << "Operation error: " << ex.what() << "\n";
        } catch (const std::exception& ex) {
            std::cout << "Operation error: " << ex.what() << "\n";
        }
    }
}

void MenuUI::showLecturerMenu(domain::Lecturer& lecturer) {
    while (true) {
        std::cout << "\n=== Lecturer Menu: " << lecturer.getName() << " ===\n"
                  << "1) Open attendance session\n"
                  << "2) Mark attendance\n"
                  << "3) View courses\n"
                  << "4) Select capture method\n"
                  << "5) Close attendance session\n"
                  << "6) View attendance report\n"
                  << "7) Correct attendance\n"
                  << "8) View enrollment list\n"
                  << "0) Logout\n"
                  << "Enter a choice: ";

        std::string choice;
        if (!readCommand(choice)) return;

        if (choice == "1") lecturer.openAttendance();
        else if (choice == "2") lecturer.markAttendance();
        else if (choice == "3") lecturer.viewCourses();
        else if (choice == "4") lecturer.selectCaptureMethod();
        else if (choice == "5") lecturer.closeAttendance();
        else if (choice == "6") lecturer.viewAttendanceReport();
        else if (choice == "7") lecturer.correctAttendance();
        else if (choice == "8") lecturer.viewEnrollmentList();
        else if (choice == "0") return;
        else std::cout << "Invalid selection.\n";
    }
}

void MenuUI::showAdminMenu(domain::Administrator& admin) {
    auto& sys = SystemManager::getInstance();
    auto read = [](const std::string& prompt) {
        std::cout << prompt; std::string v; std::getline(std::cin, v); return v;
    };
    auto toInt = [](const std::string& s, int fallback=0) {
        try { return std::stoi(s); } catch (...) { return fallback; }
    };

    while (true) {
        std::cout << "\n=== Administrator Menu ===\n"
                  << "1) Create user\n2) Edit user\n3) Remove user\n"
                  << "4) Create course\n5) Edit course\n6) Remove course\n"
                  << "7) List users\n8) List courses\n9) Enrollment summary report\n0) Logout\nEnter a choice: ";
        std::string choice; if (!readCommand(choice)) return;
        try {
            if (choice == "1") {
                std::string type=read("Type (STUDENT/LECTURER/ADMIN): ");
                std::string pid=read("Person ID: "), name=read("Name: "), email=read("Email: "), pass=read("Password: ");
                if (sys.getUserRepository().findById(pid) || sys.getUserRepository().findByEmail(email)) { std::cout << "User ID or email already exists.\n"; continue; }
                std::string details=type+"|"+pid+"|"+name+"|"+email+"|"+pass;
                if(type=="STUDENT") details += "|"+read("Student ID: ");
                else if(type=="LECTURER") details += "|"+read("Department: ")+"|"+read("Staff ID: ");
                auto made=admin.createUser(details); if(!made){ std::cout << "Invalid user details/type.\n"; continue; }
                std::shared_ptr<domain::Person> person(std::move(made)); sys.getUserRepository().add(person); sys.saveAll();
                std::cout << "User created successfully.\n";
            } else if (choice == "2") {
                auto p=sys.getUserRepository().findById(read("Person ID to edit: ")); if(!p){ std::cout<<"User not found.\n"; continue; }
                std::string v=read("New name (blank = keep): "); if(!v.empty()) p->setName(v);
                v=read("New email (blank = keep): "); if(!v.empty() && (!sys.getUserRepository().findByEmail(v) || sys.getUserRepository().findByEmail(v)==p)) p->setEmail(v);
                v=read("New password (blank = keep): "); if(!v.empty()) p->setPassword(v);
                admin.editUser(*p); sys.saveAll(); std::cout<<"User updated.\n";
            } else if (choice == "3") {
                auto p=sys.getUserRepository().findById(read("Person ID to remove: ")); if(!p){ std::cout<<"User not found.\n"; continue; }
                if(p.get()==&admin){ std::cout<<"Cannot remove the currently logged-in administrator.\n"; continue; }
                if(auto st=std::dynamic_pointer_cast<domain::Student>(p)) {
                    std::vector<std::string> codes; for(auto& e:st->getEnrollments()) if(e&&e->isActive()) codes.push_back(e->getCourse()->getCode());
                    for(auto& c:codes) st->dropCourse(c);
                }
                if(auto le=std::dynamic_pointer_cast<domain::Lecturer>(p)) for(auto& c:sys.getCourseRepository().getItems()) if(c&&c->getLecturer()==le.get()) c->setLecturer(nullptr);
                admin.removeUser(*p); sys.getUserRepository().remove(p); sys.saveAll(); std::cout<<"User removed.\n";
            } else if (choice == "4") {
                std::string type=read("Type (LECTURE/LAB/PROJECT): "), code=read("Course code: ");
                if(sys.getCourseRepository().findByCode(code)){ std::cout<<"Course code already exists.\n"; continue; }
                std::string title=read("Title: "); int cr=toInt(read("Credits: ")), cap=toInt(read("Capacity: "));
                std::string details=type+"|"+code+"|"+title+"|"+std::to_string(cr)+"|"+std::to_string(cap);
                if(type=="LAB") details += "|"+read("Lab capacity: ");
                auto made=admin.createCourse(details); if(!made){ std::cout<<"Invalid course details/type.\n"; continue; }
                made->setSchedule({read("Day: "),toInt(read("Start hour: ")),toInt(read("End hour: ")),read("Room: ")});
                std::string lecturerId=read("Lecturer Person ID or Staff ID (blank/0 = none): ");
                if(!lecturerId.empty() && lecturerId!="0") {
                    auto u=sys.getUserRepository().findById(lecturerId);
                    auto l=std::dynamic_pointer_cast<domain::Lecturer>(u);
                    if(!l) {
                        for(const auto& candidate: sys.getUserRepository().getItems()) {
                            auto lecturer=std::dynamic_pointer_cast<domain::Lecturer>(candidate);
                            if(lecturer && lecturer->getStaffId()==lecturerId) { l=lecturer; break; }
                        }
                    }
                    if(l){ made->setLecturer(l.get()); l->assignCourse(made.get()); }
                    else std::cout<<"Lecturer not found; course created unassigned.\n";
                }
                std::string pre=read("Prerequisite course code (blank/0 = none): "); if(!pre.empty() && pre!="0"){ auto pc=sys.getCourseRepository().findByCode(pre); if(pc) made->addPrerequisite(pc.get()); else std::cout<<"Prerequisite not found; skipped.\n"; }
                std::shared_ptr<domain::Course> course(std::move(made)); sys.getCourseRepository().add(course); sys.saveAll(); std::cout<<"Course created successfully.\n";
            } else if (choice == "5") {
                auto c=sys.getCourseRepository().findByCode(read("Course code to edit: ")); if(!c){ std::cout<<"Course not found.\n"; continue; }
                std::string v=read("New title (blank = keep): "); if(!v.empty()) c->setTitle(v);
                v=read("New credits (blank = keep): "); if(!v.empty()) c->setCredits(toInt(v,c->getCredits()));
                v=read("New capacity (blank = keep): "); if(!v.empty()) c->setCapacity(toInt(v,c->getCapacity()));
                v=read("Change schedule? (y/n): "); if(v=="y"||v=="Y") { auto oldSlot=c->getSchedule(); auto newSlot=attendance::TimeSlot{read("Day: "),toInt(read("Start hour: ")),toInt(read("End hour: ")),read("Room: ")}; for(auto* st:c->getEnrolledStudents()) if(st&&st->getTimetable()){ st->getTimetable()->removeSlot(oldSlot); st->getTimetable()->addSlot(newSlot); } c->setSchedule(newSlot); }
                v=read("Assign lecturer Person ID or Staff ID (blank = keep, NONE/0 = clear): ");
                if(v=="NONE" || v=="0") { if(c->getLecturer()) c->getLecturer()->removeCourse(c.get()); c->setLecturer(nullptr); } else if(!v.empty()){
                    auto u=sys.getUserRepository().findById(v); auto l=std::dynamic_pointer_cast<domain::Lecturer>(u);
                    if(!l) { for(const auto& candidate:sys.getUserRepository().getItems()) { auto lecturer=std::dynamic_pointer_cast<domain::Lecturer>(candidate); if(lecturer && lecturer->getStaffId()==v){ l=lecturer; break; } } }
                    if(l){ if(c->getLecturer() && c->getLecturer()!=l.get()) c->getLecturer()->removeCourse(c.get()); c->setLecturer(l.get()); l->assignCourse(c.get()); } else std::cout<<"Lecturer not found.\n";
                }
                v=read("Add prerequisite code (blank/0 = none): "); if(!v.empty() && v!="0"){ auto pc=sys.getCourseRepository().findByCode(v); if(pc&&pc!=c) c->addPrerequisite(pc.get()); else std::cout<<"Prerequisite not found/invalid.\n"; }
                admin.editCourse(*c); sys.saveAll(); std::cout<<"Course updated.\n";
            } else if (choice == "6") {
                auto c=sys.getCourseRepository().findByCode(read("Course code to remove: ")); if(!c){ std::cout<<"Course not found.\n"; continue; }
                std::vector<domain::Student*> students=c->getEnrolledStudents(); for(auto* st:students) if(st&&st->isEnrolledIn(c->getCode())) st->dropCourse(c->getCode());
                if(c->getLecturer()) c->getLecturer()->removeCourse(c.get());
                for(auto& other:sys.getCourseRepository().getItems()) if(other&&other!=c) other->removePrerequisite(c.get());
                admin.removeCourse(*c); sys.getCourseRepository().remove(c); sys.saveAll(); std::cout<<"Course removed.\n";
            } else if (choice == "7") {
                std::cout<<"\nUsers:\n"; for(auto& p:sys.getUserRepository().getItems()) if(p) std::cout << "  " << *p << "\n";
            } else if (choice == "8") {
                std::cout<<"\nCourses:\n"; for(auto& c:sys.getCourseRepository().getItems()) if(c) std::cout << "  " << *c << "\n";
            } else if (choice == "9") {
                std::cout << "\n--- Enrollment Summary Report ---\n";
                for (const auto& c : sys.getCourseRepository().getItems()) if (c)
                    std::cout << c->getCode() << " | " << c->getTitle() << " | "
                              << c->getEnrolledStudents().size() << "/" << c->getCapacity() << " enrolled\n";
            } else if (choice == "0") return;
            else std::cout<<"Invalid selection.\n";
        } catch(const std::exception& ex) { std::cout<<"Admin operation error: "<<ex.what()<<"\n"; }
    }
}

} // namespace repo
