#pragma once
#include <vector>
#include "Common.hpp"

class MaxHeap {
public:
    void push(const EmergencyEvent& e);
    bool empty() const { return data_.empty(); }
    std::size_t size() const { return data_.size(); }

    EmergencyEvent popMax(); // throws if empty
    const EmergencyEvent& peekMax() const; // throws if empty

private:
    std::vector<EmergencyEvent> data_;

    static bool higher(const EmergencyEvent& a, const EmergencyEvent& b) {
        return a.severity > b.severity;
    }

    void siftUp(std::size_t i);
    void siftDown(std::size_t i);
};

