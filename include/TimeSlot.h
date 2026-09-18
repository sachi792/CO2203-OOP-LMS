#pragma once
#include <string>

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

    // Checks if two classes are on the same day and their times overlap.
    bool overlaps(const TimeSlot& other) const noexcept {
        return day == other.day && startHour < other.endHour && other.startHour < endHour;
    }
};

}
