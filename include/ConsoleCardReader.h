#pragma once
#include <string>

namespace attendance {

// Reads a card UID from the console for testing
class ConsoleCardReader {
public:
    // Returns the entered UID. Empty string means the input has ended or "q" was entered.
    std::string readUID();
    
};

}
