#include "Timetable.h"
#include <algorithm>
#include <iostream>

namespace attendance {

void Timetable::addSlot(const TimeSlot& slot) {
    slots_.push_back(slot);
}

void Timetable::removeSlot(const TimeSlot& slot) {
    slots_.erase(std::remove(slots_.begin(), slots_.end(), slot), slots_.end());
}

void Timetable::display() const {
    std::cout << *this;
}

std::optional<TimeSlot> Timetable::clashesWith(const TimeSlot& candidate) const {
    for (const auto& slot : slots_) {
        if (slot.overlaps(candidate)) {
            return slot;
        }
    }
    return std::nullopt;
}

const std::vector<TimeSlot>& Timetable::getSlots() const noexcept {
    return slots_;
}

std::ostream& operator<<(std::ostream& os, const Timetable& timetable) {
    for (const auto& slot : timetable.slots_) {
        os << slot.day << " " << slot.startHour << "-" << slot.endHour
           << " (" << slot.room << ")\n";
    }
    return os;
}

}
