#include "modules/CitizenDB.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void CitizenDB::insert(const Citizen& c) {
    if (mode_ == Mode::BSTMode) bst_.insert(c);
    else avl_.insert(c);
}

bool CitizenDB::erase(int id) {
    if (mode_ == Mode::BSTMode) return bst_.erase(id);
    return avl_.erase(id);
}

void CitizenDB::findAndPrint(int id) const {
    auto res = (mode_ == Mode::BSTMode) ? bst_.find(id) : avl_.find(id);
    if (!res) {
        std::cout << "Citizen not found.\n";
        return;
    }
    std::cout << "Citizen: ID=" << res->id << ", Name=" << res->name << ", Age=" << res->age << "\n";
}

void CitizenDB::loadFromFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        std::cout << "Failed to open: " << path << "\n";
        return;
    }

    // Load into BOTH trees so you can toggle without losing data
    // (and compare performance in a consistent way).
    std::vector<Citizen> citizens;
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string sid, name, sage;
        if (!std::getline(ss, sid, ';')) continue;
        if (!std::getline(ss, name, ';')) continue;
        if (!std::getline(ss, sage, ';')) continue;
        Citizen c;
        c.id = std::stoi(sid);
        c.name = name;
        c.age = std::stoi(sage);
        citizens.push_back(c);
    }

    long long bstMs = timeMs([&](){ for (auto& c : citizens) bst_.insert(c); });
    long long avlMs = timeMs([&](){ for (auto& c : citizens) avl_.insert(c); });

    std::cout << "Loaded " << citizens.size() << " citizens.\n";
    std::cout << "Insert timing (ms): BST=" << bstMs << ", AVL=" << avlMs << "\n";
}

void CitizenDB::printStats() const {
    std::cout << "BST: size=" << bst_.size() << ", height=" << bst_.height() << "\n";
    std::cout << "AVL: size=" << avl_.size() << ", height=" << avl_.height() << "\n";
    std::cout << "Current mode: " << (mode_==Mode::BSTMode ? "BST" : "AVL") << "\n";
}

