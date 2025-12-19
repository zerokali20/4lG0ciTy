#pragma once
#include <string>
#include <vector>
#include <optional>
#include "structures/hash/HashTable.hpp"

class Graph {
public:
    explicit Graph(bool directed=false);

    int addNode(const std::string& name);
    void addEdge(const std::string& from, const std::string& to, int weight);

    int nodeId(const std::string& name) const;                 // throws if missing
    std::optional<int> nodeIdOpt(const std::string& name) const;
    const std::string& nodeName(int id) const;

    bool directed() const { return directed_; }
    int V() const { return static_cast<int>(adj_.size()); }

    struct Edge { int to; int w; };

    const std::vector<Edge>& neighbors(int id) const { return adj_[id]; }

private:
    bool directed_;
    HashTable<int> idOf_;                // node name -> id
    std::vector<std::string> nameOf_;    // id -> name
    std::vector<std::vector<Edge>> adj_; // adjacency list
};

