#include "StudentCard.h"

namespace attendance {

StudentCard::StudentCard(std::string uid, domain::Student* owner)
    : uid_(std::move(uid)), owner_(owner) {}

const std::string& StudentCard::getUID() const noexcept { return uid_; }
domain::Student* StudentCard::getOwner() const noexcept { return owner_; }

} 
