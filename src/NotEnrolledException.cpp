#include "NotEnrolledException.h"

namespace domain {

NotEnrolledException::NotEnrolledException(std::string studentId, std::string courseCode)
    : LMSException("Student " + studentId + " is not enrolled in " + courseCode),
      studentId_(std::move(studentId)), courseCode_(std::move(courseCode)) {}

const std::string& NotEnrolledException::getStudentId() const noexcept { return studentId_; }
const std::string& NotEnrolledException::getCourseCode() const noexcept { return courseCode_; }

} 
