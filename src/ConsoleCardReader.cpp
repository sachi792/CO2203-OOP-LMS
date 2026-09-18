#include "ConsoleCardReader.h"
#include <iostream>

namespace attendance {

std::string ConsoleCardReader::readUID() {
    std::string line;
    if (!std::getline(std::cin, line) || line == "q") {
        return "";
    }
    return line;
}

}
