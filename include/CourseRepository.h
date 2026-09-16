#pragma once
#include "Repository.h"
#include "Course.h"
#include <memory>
#include <string>

namespace repo {

// Repository<std::shared_ptr<domain::Course>>, serializing
// LectureBasedCourse/LabBasedCourse/ProjectBasedCourse via a type tag,
// the same pattern as UserRepository.
class CourseRepository : public Repository<std::shared_ptr<domain::Course>> {
public:
    explicit CourseRepository(const std::string& filePath = "courses.dat");

    std::shared_ptr<domain::Course> findByCode(const std::string& courseCode) const;

private:
    static std::string toRecord(const std::shared_ptr<domain::Course>& course);
    static std::shared_ptr<domain::Course> fromRecord(const std::string& record);
};

} // namespace repo
