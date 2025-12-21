#pragma once
#include "structures/tree/Heap.hpp"

class EmergencySystem {
public:
    void report(const EmergencyEvent& e) { heap_.push(e); }
    void dispatchNext();

private:
    MaxHeap heap_;
};

