#pragma once
#include "LMSException.h"
#include "TimeSlot.h"

namespace domain {

using attendance::TimeSlot;

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

}
