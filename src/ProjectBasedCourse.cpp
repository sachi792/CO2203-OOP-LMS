#include "ProjectBasedCourse.h"

namespace domain {

// Calculates the grade using the lab course marking scheme.
Grade ProjectBasedCourse::calculateGrade(double marks) const {
    if (marks >= 75) return Grade::A;
    if (marks >= 65) return Grade::B;
    if (marks >= 50) return Grade::C;
    return Grade::F;
}

}
