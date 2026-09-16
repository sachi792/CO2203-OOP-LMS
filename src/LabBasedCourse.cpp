#include "LabBasedCourse.h"

namespace domain {

LabBasedCourse::LabBasedCourse(std::string courseCode, std::string title, int creditValue,
                                int capacity, int labCapacity)
    : Course(std::move(courseCode), std::move(title), creditValue, capacity),
      labCapacity_(labCapacity) {}

// Lab courses weight practical competency more heavily -> slightly more
// forgiving pass mark than a pure lecture course.
Grade LabBasedCourse::calculateGrade(double marks) const {
    if (marks >= 65) return Grade::A;
    if (marks >= 55) return Grade::B;
    if (marks >= 45) return Grade::C;
    if (marks >= 35) return Grade::D;
    return Grade::F;
}

int LabBasedCourse::getLabCapacity() const noexcept { return labCapacity_; }

} // namespace domain
