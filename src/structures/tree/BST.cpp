#include "structures/tree/BST.hpp"

BST::~BST() { destroy(root_); }

void BST::destroy(Node* n) {
    if (!n) return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}

BST::Node* BST::insert(Node* n, const Citizen& c) {
    if (!n) { ++size_; return new Node{c}; }
    if (c.id < n->data.id) n->left = insert(n->left, c);
    else if (c.id > n->data.id) n->right = insert(n->right, c);
    else n->data = c; // overwrite
    return n;
}

void BST::insert(const Citizen& c) { root_ = insert(root_, c); }

BST::Node* BST::minNode(Node* n) const {
    while (n && n->left) n = n->left;
    return n;
}

BST::Node* BST::erase(Node* n, int id, bool& removed) {
    if (!n) return nullptr;

    if (id < n->data.id) n->left = erase(n->left, id, removed);
    else if (id > n->data.id) n->right = erase(n->right, id, removed);
    else {
        removed = true;
        if (!n->left) {
            Node* r = n->right;
            delete n;
            return r;
        }
        if (!n->right) {
            Node* l = n->left;
            delete n;
            return l;
        }
        Node* succ = minNode(n->right);
        n->data = succ->data;
        n->right = erase(n->right, succ->data.id, removed); // removed stays true
    }
    return n;
}

bool BST::erase(int id) {
    bool removed = false;
    root_ = erase(root_, id, removed);
    if (removed) --size_;
    return removed;
}

BST::Node* BST::findNode(Node* n, int id) const {
    while (n) {
        if (id < n->data.id) n = n->left;
        else if (id > n->data.id) n = n->right;
        else return n;
    }
    return nullptr;
}

std::optional<Citizen> BST::find(int id) const {
    Node* n = findNode(root_, id);
    if (!n) return std::nullopt;
    return n->data;
}

int BST::height(Node* n) const {
    if (!n) return 0;
    int lh = height(n->left);
    int rh = height(n->right);
    return 1 + (lh > rh ? lh : rh);
}

int BST::height() const { return height(root_); }

