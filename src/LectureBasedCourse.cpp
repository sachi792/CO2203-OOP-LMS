#include "LectureBasedCourse.h"

namespace domain {
    
// Calculates the grade using the lecture course marking scheme.
Grade LectureBasedCourse::calculateGrade(double marks) const {
    if (marks >= 70) return Grade::A;
    if (marks >= 60) return Grade::B;
    if (marks >= 50) return Grade::C;
    if (marks >= 40) return Grade::D;
    return Grade::F;
}

}
