#pragma once
#include "LMSException.h"
#include "TimeSlot.h"

namespace domain {

// TimeSlot now canonically lives in the attendance module (see
// CO2203_Attendance/README.md, suggestion #5) - this brings that same type
// into scope here under its unqualified name, rather than domain defining
// its own competing copy.
using attendance::TimeSlot;

// Thrown when Enrollment::enroll() detects the course's timetable slot
// collides with a slot the student is already committed to.
class TimetableClashException : public LMSException {
public:
    TimetableClashException(TimeSlot slotA, TimeSlot slotB);

    const TimeSlot& getSlotA() const noexcept;
    const TimeSlot& getSlotB() const noexcept;

private:
    TimeSlot slotA_;
    TimeSlot slotB_;

    static std::string buildMessage(const TimeSlot& a, const TimeSlot& b);
};

} // namespace domain
