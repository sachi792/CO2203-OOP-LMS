#pragma once
#include <string>

namespace attendance {

// Simulates a physical card-tap reader using the console, for demo/testing
// purposes until real hardware integration exists.
class ConsoleCardReader {
public:
    // Reads one line from stdin representing a tapped card's UID.
    // Returns an empty string if input ends (EOF) or the user types "q".
    std::string readUID();
};

} // namespace attendance
