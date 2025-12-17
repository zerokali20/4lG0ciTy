#pragma once
#include <functional>
#include <optional>
#include "Common.hpp"

class BST {
public:
    BST() = default;
    ~BST();

    void insert(const Citizen& c);
    bool erase(int id);
    std::optional<Citizen> find(int id) const;

    // For benchmarking / debugging
    int height() const;
    std::size_t size() const { return size_; }

private:
    struct Node {
        Citizen data;
        Node* left{nullptr};
        Node* right{nullptr};
    };

    Node* root_{nullptr};
    std::size_t size_{0};

    Node* insert(Node* n, const Citizen& c);
    Node* erase(Node* n, int id, bool& removed);
    Node* minNode(Node* n) const;

    Node* findNode(Node* n, int id) const;

    int height(Node* n) const;
    void destroy(Node* n);
};

