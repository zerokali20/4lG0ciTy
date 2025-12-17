#include "structures/tree/AVLTree.hpp"

AVLTree::~AVLTree() { destroy(root_); }

void AVLTree::destroy(Node* n) {
    if (!n) return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}

AVLTree::Node* AVLTree::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->h = 1 + std::max(height(y->left), height(y->right));
    x->h = 1 + std::max(height(x->left), height(x->right));
    return x;
}

AVLTree::Node* AVLTree::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->h = 1 + std::max(height(x->left), height(x->right));
    y->h = 1 + std::max(height(y->left), height(y->right));
    return y;
}

AVLTree::Node* AVLTree::insert(Node* n, const Citizen& c) {
    if (!n) { ++size_; return new Node{c}; }

    if (c.id < n->data.id) n->left = insert(n->left, c);
    else if (c.id > n->data.id) n->right = insert(n->right, c);
    else { n->data = c; return n; }

    n->h = 1 + std::max(height(n->left), height(n->right));
    int b = balance(n);

    // LL
    if (b > 1 && c.id < n->left->data.id) return rotateRight(n);
    // RR
    if (b < -1 && c.id > n->right->data.id) return rotateLeft(n);
    // LR
    if (b > 1 && c.id > n->left->data.id) { n->left = rotateLeft(n->left); return rotateRight(n); }
    // RL
    if (b < -1 && c.id < n->right->data.id) { n->right = rotateRight(n->right); return rotateLeft(n); }

    return n;
}

void AVLTree::insert(const Citizen& c) { root_ = insert(root_, c); }

AVLTree::Node* AVLTree::minNode(Node* n) const {
    while (n && n->left) n = n->left;
    return n;
}

AVLTree::Node* AVLTree::erase(Node* n, int id, bool& removed) {
    if (!n) return nullptr;

    if (id < n->data.id) n->left = erase(n->left, id, removed);
    else if (id > n->data.id) n->right = erase(n->right, id, removed);
    else {
        removed = true;
        if (!n->left || !n->right) {
            Node* child = n->left ? n->left : n->right;
            delete n;
            return child;
        }
        Node* succ = minNode(n->right);
        n->data = succ->data;
        n->right = erase(n->right, succ->data.id, removed);
    }

    if (!n) return nullptr;

    n->h = 1 + std::max(height(n->left), height(n->right));
    int b = balance(n);

    // LL
    if (b > 1 && balance(n->left) >= 0) return rotateRight(n);
    // LR
    if (b > 1 && balance(n->left) < 0) { n->left = rotateLeft(n->left); return rotateRight(n); }
    // RR
    if (b < -1 && balance(n->right) <= 0) return rotateLeft(n);
    // RL
    if (b < -1 && balance(n->right) > 0) { n->right = rotateRight(n->right); return rotateLeft(n); }

    return n;
}

bool AVLTree::erase(int id) {
    bool removed = false;
    root_ = erase(root_, id, removed);
    if (removed) --size_;
    return removed;
}

AVLTree::Node* AVLTree::findNode(Node* n, int id) const {
    while (n) {
        if (id < n->data.id) n = n->left;
        else if (id > n->data.id) n = n->right;
        else return n;
    }
    return nullptr;
}

std::optional<Citizen> AVLTree::find(int id) const {
    Node* n = findNode(root_, id);
    if (!n) return std::nullopt;
    return n->data;
}

int AVLTree::height() const { return height(root_); }

