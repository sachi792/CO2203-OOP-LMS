#include "UserRepository.h"
#include "FileStorage.h"
#include "Student.h"
#include "Lecturer.h"
#include "Administrator.h"
#include <sstream>
#include <vector>
#include <iostream>

namespace repo {

namespace {
                        
std::vector<std::string> splitFields(const std::string& record) {
    std::vector<std::string> fields;
    std::stringstream ss(record);
    std::string field;
    while (std::getline(ss, field, '|')) {
        fields.push_back(field);
    }
    return fields;
}

} 

UserRepository::UserRepository(const std::string& filePath)
    : Repository(std::make_shared<FileStorage>(filePath), &UserRepository::toRecord, &UserRepository::fromRecord) {}

std::shared_ptr<domain::Person> UserRepository::findByEmail(const std::string& email) const {
    for (const auto& person : getItems()) {
        if (person->getEmail() == email) return person;
    }
    return nullptr;
}

std::shared_ptr<domain::Person> UserRepository::findById(const std::string& personId) const {
    for (const auto& person : getItems()) {
        if (person->getPersonId() == personId) return person;
    }
    return nullptr;
}

std::string UserRepository::toRecord(const std::shared_ptr<domain::Person>& person) {
    if (auto student = std::dynamic_pointer_cast<domain::Student>(person)) {
        return "STUDENT|" + student->getPersonId() + "|" + student->getName() + "|" +
               student->getEmail() + "|" + student->getPassword() + "|" + student->getStudentId();
    }
    if (auto lecturer = std::dynamic_pointer_cast<domain::Lecturer>(person)) {
        return "LECTURER|" + lecturer->getPersonId() + "|" + lecturer->getName() + "|" +
               lecturer->getEmail() + "|" + lecturer->getPassword() + "|" + lecturer->getDepartment() + "|" + lecturer->getStaffId();
    }
    if (auto admin = std::dynamic_pointer_cast<domain::Administrator>(person)) {
        return "ADMIN|" + admin->getPersonId() + "|" + admin->getName() + "|" + admin->getEmail() + "|" + admin->getPassword();
    }
    std::cerr << "[UserRepository] Unknown Person subtype - cannot serialize\n";
    return "";
}

std::shared_ptr<domain::Person> UserRepository::fromRecord(const std::string& record) {
    auto fields = splitFields(record);
    if (fields.empty()) return nullptr;

    const std::string& type = fields[0];
    // fields: [0]=type [1]=personId [2]=name [3]=email [4]=password(blank) [5..]=type-specific
    if (type == "STUDENT" && fields.size() >= 6) {
        return std::make_shared<domain::Student>(fields[1], fields[2], fields[3], fields[4], fields[5]);
    }
    if (type == "LECTURER" && fields.size() >= 7) {
        return std::make_shared<domain::Lecturer>(fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]);
    }
    if (type == "ADMIN" && fields.size() >= 5) {
        return std::make_shared<domain::Administrator>(fields[1], fields[2], fields[3], fields[4]);
    }
    std::cerr << "[UserRepository] Unrecognised record, skipping: " << record << "\n";
    return nullptr;
}

} 
