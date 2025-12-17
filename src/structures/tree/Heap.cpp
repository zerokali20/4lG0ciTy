#include "structures/tree/Heap.hpp"
#include <stdexcept>

void MaxHeap::push(const EmergencyEvent& e) {
    data_.push_back(e);
    siftUp(data_.size() - 1);
}

const EmergencyEvent& MaxHeap::peekMax() const {
    if (data_.empty()) throw std::runtime_error("Heap empty");
    return data_[0];
}

EmergencyEvent MaxHeap::popMax() {
    if (data_.empty()) throw std::runtime_error("Heap empty");
    EmergencyEvent top = data_[0];
    data_[0] = data_.back();
    data_.pop_back();
    if (!data_.empty()) siftDown(0);
    return top;
}

void MaxHeap::siftUp(std::size_t i) {
    while (i > 0) {
        std::size_t p = (i - 1) / 2;
        if (higher(data_[p], data_[i])) break;
        std::swap(data_[p], data_[i]);
        i = p;
    }
}

void MaxHeap::siftDown(std::size_t i) {
    std::size_t n = data_.size();
    while (true) {
        std::size_t l = 2 * i + 1;
        std::size_t r = 2 * i + 2;
        std::size_t best = i;

        if (l < n && higher(data_[l], data_[best])) best = l;
        if (r < n && higher(data_[r], data_[best])) best = r;

        if (best == i) break;
        std::swap(data_[i], data_[best]);
        i = best;
    }
}

