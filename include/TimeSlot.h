#pragma once
#include <string>

// ============================================================================
// TimeSlot.h
//
// Canonical home: this is the ONE definition of TimeSlot for the whole
// merged project. domain::TimetableClashException and domain::Course both
// bring this into their own namespace via `using attendance::TimeSlot;`
// rather than defining a competing copy.
// ============================================================================

namespace attendance {

struct TimeSlot {
    std::string day;
    int startHour = 0;
    int endHour = 0;
    std::string room;

    bool operator==(const TimeSlot& other) const noexcept {
        return day == other.day &&
               startHour == other.startHour &&
               endHour == other.endHour &&
               room == other.room;
    }

    bool operator!=(const TimeSlot& other) const noexcept {
        return !(*this == other);
    }

    // Two slots clash if they're on the same day and their hour ranges overlap.
    bool overlaps(const TimeSlot& other) const noexcept {
        return day == other.day && startHour < other.endHour && other.startHour < endHour;
    }
};

} // namespace attendance
