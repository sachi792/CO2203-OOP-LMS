#include "CourseRepository.h"
#include "FileStorage.h"
#include "LectureBasedCourse.h"
#include "LabBasedCourse.h"
#include "ProjectBasedCourse.h"
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
} // namespace

CourseRepository::CourseRepository(const std::string& filePath)
    : Repository(std::make_shared<FileStorage>(filePath), &CourseRepository::toRecord, &CourseRepository::fromRecord) {}

std::shared_ptr<domain::Course> CourseRepository::findByCode(const std::string& courseCode) const {
    for (const auto& course : getItems()) {
        if (course->getCode() == courseCode) return course;
    }
    return nullptr;
}

// Record formats:
//   LECTURE|code|title|credits|capacity
//   PROJECT|code|title|credits|capacity
//   LAB|code|title|credits|capacity|labCapacity
//
// NOTE (flagged in README): this only persists each course's own scalar
// fields. Relationships - assigned lecturer, prerequisites, enrolled
// students - are pointers to OTHER objects and are intentionally not
// serialized here; re-linking them after loadAll() is a second pass
// SystemManager would need to orchestrate once ALL repositories are
// loaded (you can't resolve a "prerequisite: CO2203" reference until the
// Course for CO2203 has itself been deserialized).
std::string CourseRepository::toRecord(const std::shared_ptr<domain::Course>& course) {
    if (auto lab = std::dynamic_pointer_cast<domain::LabBasedCourse>(course)) {
        return "LAB|" + lab->getCode() + "|" + lab->getTitle() + "|" +
               std::to_string(lab->getCredits()) + "|" + std::to_string(lab->getCapacity()) + "|" + std::to_string(lab->getLabCapacity()) + "|" + lab->getSchedule().day + "|" + std::to_string(lab->getSchedule().startHour) + "|" + std::to_string(lab->getSchedule().endHour) + "|" + lab->getSchedule().room;
    }
    if (auto project = std::dynamic_pointer_cast<domain::ProjectBasedCourse>(course)) {
        return "PROJECT|" + project->getCode() + "|" + project->getTitle() + "|" +
               std::to_string(project->getCredits()) + "|" + std::to_string(project->getCapacity()) + "|" + project->getSchedule().day + "|" + std::to_string(project->getSchedule().startHour) + "|" + std::to_string(project->getSchedule().endHour) + "|" + project->getSchedule().room;
    }
    if (auto lecture = std::dynamic_pointer_cast<domain::LectureBasedCourse>(course)) {
        return "LECTURE|" + lecture->getCode() + "|" + lecture->getTitle() + "|" +
               std::to_string(lecture->getCredits()) + "|" + std::to_string(lecture->getCapacity()) + "|" + lecture->getSchedule().day + "|" + std::to_string(lecture->getSchedule().startHour) + "|" + std::to_string(lecture->getSchedule().endHour) + "|" + lecture->getSchedule().room;
    }
    std::cerr << "[CourseRepository] Unknown Course subtype - cannot serialize\n";
    return "";
}

std::shared_ptr<domain::Course> CourseRepository::fromRecord(const std::string& record) {
    auto fields = splitFields(record);
    if (fields.empty()) return nullptr;

    const std::string& type = fields[0];
    try {
        if (type == "LECTURE" && fields.size() >= 5) {
            auto c = std::make_shared<domain::LectureBasedCourse>(fields[1], fields[2], std::stoi(fields[3]), std::stoi(fields[4]));
            if (fields.size() >= 9) c->setSchedule({fields[5], std::stoi(fields[6]), std::stoi(fields[7]), fields[8]});
            return c;
        }
        if (type == "PROJECT" && fields.size() >= 5) {
            auto c = std::make_shared<domain::ProjectBasedCourse>(fields[1], fields[2], std::stoi(fields[3]), std::stoi(fields[4]));
            if (fields.size() >= 9) c->setSchedule({fields[5], std::stoi(fields[6]), std::stoi(fields[7]), fields[8]});
            return c;
        }
        if (type == "LAB" && fields.size() >= 6) {
            auto c = std::make_shared<domain::LabBasedCourse>(fields[1], fields[2], std::stoi(fields[3]), std::stoi(fields[4]), std::stoi(fields[5]));
            if (fields.size() >= 10) c->setSchedule({fields[6], std::stoi(fields[7]), std::stoi(fields[8]), fields[9]});
            return c;
        }
    } catch (const std::exception& ex) {
        std::cerr << "[CourseRepository] Malformed record, skipping: " << record << " (" << ex.what() << ")\n";
        return nullptr;
    }
    std::cerr << "[CourseRepository] Unrecognised record, skipping: " << record << "\n";
    return nullptr;
}

} // namespace repo
