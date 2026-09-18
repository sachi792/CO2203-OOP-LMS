#pragma once
#include "Course.h"

namespace domain {

class LectureBasedCourse : public Course {
public:
    using Course::Course; // use the same constructor from Course

    Grade calculateGrade(double marks) const override;
};

}
