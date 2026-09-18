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

   // Returns the first slot that overlaps with the given slot.
    // Returns std::nullopt if there is no clash.
    std::optional<TimeSlot> clashesWith(const TimeSlot& candidate) const;

    const std::vector<TimeSlot>& getSlots() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const Timetable& timetable);

private:
    std::vector<TimeSlot> slots_;
};

} // namespace attendance
