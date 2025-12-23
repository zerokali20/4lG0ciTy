#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <iostream>

struct Citizen {
    int id{};
    std::string name;
    int age{};
};

struct EmergencyEvent {
    int severity{};           // higher = more urgent
    std::string type;         // fire/medical/robbery...
    std::string location;     // node name
    std::string details;
};

