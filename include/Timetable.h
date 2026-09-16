#pragma once
#include <vector>
#include <optional>
#include <ostream>
#include "TimeSlot.h"

namespace attendance {

class Timetable {
public:
    void addSlot(const TimeSlot& slot);
    void removeSlot(const TimeSlot& slot);
    void display() const;

    // Suggested addition (requested by Member 1's Enrollment::checkTimetableClash()
    // integration stub): returns the first existing slot that overlaps
    // `candidate`, or std::nullopt if there's no clash.
    std::optional<TimeSlot> clashesWith(const TimeSlot& candidate) const;

    const std::vector<TimeSlot>& getSlots() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const Timetable& timetable);

private:
    std::vector<TimeSlot> slots_;
};

} // namespace attendance
