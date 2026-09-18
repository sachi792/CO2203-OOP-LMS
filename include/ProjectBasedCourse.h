#pragma once
#include "Course.h"

namespace domain {

class ProjectBasedCourse : public Course {
public:
    using Course::Course;

    Grade calculateGrade(double marks) const override;
};

} 
