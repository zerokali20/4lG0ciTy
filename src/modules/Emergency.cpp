#include "modules/Emergency.hpp"
#include <iostream>

void EmergencySystem::dispatchNext() {
    if (heap_.empty()) {
        std::cout << "No pending emergencies.\n";
        return;
    }
    auto e = heap_.popMax();
    std::cout << "DISPATCHING: severity=" << e.severity
              << ", type=" << e.type
              << ", location=" << e.location
              << "\nDetails: " << e.details << "\n";
}

