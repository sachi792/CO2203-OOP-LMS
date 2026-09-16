#pragma once
#include "Course.h"

namespace domain {

class LectureBasedCourse : public Course {
public:
    using Course::Course; // inherit the 4-argument constructor as-is

    Grade calculateGrade(double marks) const override;
};

} // namespace domain
