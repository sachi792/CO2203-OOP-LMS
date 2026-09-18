#pragma once
#include "Course.h"

namespace domain {

class LabBasedCourse : public Course {
public:
    LabBasedCourse(std::string courseCode, std::string title, int creditValue, int capacity,
                    int labCapacity);

    Grade calculateGrade(double marks) const override;

    int getLabCapacity() const noexcept;

private:
    int labCapacity_;
};

}
