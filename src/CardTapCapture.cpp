#include "CardTapCapture.h"
#include <iostream>

namespace attendance {

void CardTapCapture::beginSession() {
    std::cout << "[CardTapCapture] Reader ready - tap a card (or type 'q' to stop)\n";
}

std::optional<std::string> CardTapCapture::captureNext() {
    std::string uid = reader_.readUID();
    if (uid.empty()) {
        return std::nullopt;
    }
    return uid;
}

void CardTapCapture::endSession() {
    std::cout << "[CardTapCapture] Reader closed\n";
}

}
