#pragma once
#include "Repository.h"
#include "Course.h"
#include <memory>
#include <string>

namespace repo {

    // Stores different course types using a type tag when saving them.
class CourseRepository : public Repository<std::shared_ptr<domain::Course>> {
public:
    explicit CourseRepository(const std::string& filePath = "courses.dat");

    std::shared_ptr<domain::Course> findByCode(const std::string& courseCode) const;

private:
    // Converts course to a string for saving.
    static std::string toRecord(const std::shared_ptr<domain::Course>& course);

    // Converts  saved string back to a course object.
    static std::shared_ptr<domain::Course> fromRecord(const std::string& record);
};

} 
