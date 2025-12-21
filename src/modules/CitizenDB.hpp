#pragma once
#include <string>
#include <chrono>
#include "Common.hpp"
#include "structures/tree/BST.hpp"
#include "structures/tree/AVLTree.hpp"

class CitizenDB {
public:
    enum class Mode { BSTMode, AVLMode };

    void setMode(Mode m) { mode_ = m; }
    Mode mode() const { return mode_; }

    void insert(const Citizen& c);
    bool erase(int id);
    void findAndPrint(int id) const;

    void loadFromFile(const std::string& path);
    void printStats() const;

private:
    Mode mode_{Mode::AVLMode};
    BST bst_;
    AVLTree avl_;

    template <typename Fn>
    static long long timeMs(Fn&& fn) {
        auto t0 = std::chrono::high_resolution_clock::now();
        fn();
        auto t1 = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    }
};

