#pragma once
#include <string>
#include <vector>
#include <optional>
#include <utility>
#include <cstddef>
#include "structures/hash/HashUtils.hpp"

// Custom hash table for string keys using separate chaining.
// - No std::unordered_map
// - resize() doubles capacity when load factor > 0.7

template <typename V>
class HashTable {
public:
    HashTable(std::size_t initial_capacity = 16)
        : buckets_(HashUtils::nextPow2(initial_capacity)), size_(0) {}

    bool contains(const std::string& key) const {
        return findNode(key) != nullptr;
    }

    // Insert or overwrite
    void put(const std::string& key, const V& value) {
        maybeResize();
        std::size_t idx = indexFor(key);
        Node* cur = buckets_[idx];
        while (cur) {
            if (cur->key == key) { cur->value = value; return; }
            cur = cur->next;
        }
        buckets_[idx] = new Node{key, value, buckets_[idx]};
        ++size_;
    }

    // Returns std::optional<V> (copy)
    std::optional<V> get(const std::string& key) const {
        Node* n = findNode(key);
        if (!n) return std::nullopt;
        return n->value;
    }

    // Like get(), but throws if missing (convenient in algorithms)
    const V& at(const std::string& key) const {
        Node* n = findNode(key);
        if (!n) throw std::out_of_range("HashTable: key not found: " + key);
        return n->value;
    }

    bool erase(const std::string& key) {
        std::size_t idx = indexFor(key);
        Node* cur = buckets_[idx];
        Node* prev = nullptr;
        while (cur) {
            if (cur->key == key) {
                if (prev) prev->next = cur->next;
                else buckets_[idx] = cur->next;
                delete cur;
                --size_;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return buckets_.size(); }
    double loadFactor() const {
        return buckets_.empty() ? 0.0 : static_cast<double>(size_) / static_cast<double>(buckets_.size());
    }

    void clear() {
        for (auto& head : buckets_) {
            Node* cur = head;
            while (cur) {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
            head = nullptr;
        }
        size_ = 0;
    }

    ~HashTable() { clear(); }

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

private:
    struct Node {
        std::string key;
        V value;
        Node* next;
    };

    std::vector<Node*> buckets_;
    std::size_t size_;

    std::size_t indexFor(const std::string& key) const {
        std::uint64_t h = HashUtils::fnv1a64(key);
        return static_cast<std::size_t>(h) & (buckets_.size() - 1); // power-of-two mask
    }

    Node* findNode(const std::string& key) const {
        if (buckets_.empty()) return nullptr;
        std::size_t idx = indexFor(key);
        Node* cur = buckets_[idx];
        while (cur) {
            if (cur->key == key) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    void maybeResize() {
        if (loadFactor() <= 0.7) return;
        rehash(buckets_.size() * 2);
    }

    void rehash(std::size_t new_capacity) {
        new_capacity = HashUtils::nextPow2(new_capacity);
        std::vector<Node*> newBuckets(new_capacity, nullptr);

        for (Node* head : buckets_) {
            Node* cur = head;
            while (cur) {
                Node* nxt = cur->next;
                std::uint64_t h = HashUtils::fnv1a64(cur->key);
                std::size_t idx = static_cast<std::size_t>(h) & (new_capacity - 1);
                cur->next = newBuckets[idx];
                newBuckets[idx] = cur;
                cur = nxt;
            }
        }
        buckets_.swap(newBuckets);
    }
};

