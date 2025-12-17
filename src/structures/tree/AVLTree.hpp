#pragma once
#include <optional>
#include "Common.hpp"

class AVLTree {
public:
    AVLTree() = default;
    ~AVLTree();

    void insert(const Citizen& c);
    bool erase(int id);
    std::optional<Citizen> find(int id) const;

    int height() const;
    std::size_t size() const { return size_; }

private:
    struct Node {
        Citizen data;
        Node* left{nullptr};
        Node* right{nullptr};
        int h{1};
    };

    Node* root_{nullptr};
    std::size_t size_{0};

    int height(Node* n) const { return n ? n->h : 0; }
    int balance(Node* n) const { return n ? height(n->left) - height(n->right) : 0; }

    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);

    Node* insert(Node* n, const Citizen& c);
    Node* erase(Node* n, int id, bool& removed);

    Node* minNode(Node* n) const;
    Node* findNode(Node* n, int id) const;

    void destroy(Node* n);
};

