#include "ProjectBasedCourse.h"

namespace domain {

// Project courses are typically pass/fail-oriented with a higher bar,
// as there is no exam component to average against.
Grade ProjectBasedCourse::calculateGrade(double marks) const {
    if (marks >= 75) return Grade::A;
    if (marks >= 65) return Grade::B;
    if (marks >= 50) return Grade::C;
    return Grade::F;
}

} // namespace domain
