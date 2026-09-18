#include "TimetableClashException.h"

namespace domain {

TimetableClashException::TimetableClashException(TimeSlot slotA, TimeSlot slotB)
    : LMSException(buildMessage(slotA, slotB)),
      slotA_(std::move(slotA)), slotB_(std::move(slotB)) {}

const TimeSlot& TimetableClashException::getSlotA() const noexcept { return slotA_; }
const TimeSlot& TimetableClashException::getSlotB() const noexcept { return slotB_; }

std::string TimetableClashException::buildMessage(const TimeSlot& a, const TimeSlot& b) {
    return "Timetable clash on " + a.day + " (" + a.room + ") vs " + b.day + " (" + b.room + ")";
}

}
