#include "structures/graph/Graph.hpp"
#include <stdexcept>

Graph::Graph(bool directed) : directed_(directed), idOf_(32) {}

int Graph::addNode(const std::string& name) {
    auto id = idOf_.get(name);
    if (id.has_value()) return *id;

    int nid = static_cast<int>(adj_.size());
    idOf_.put(name, nid);
    nameOf_.push_back(name);
    adj_.push_back({});
    return nid;
}

void Graph::addEdge(const std::string& from, const std::string& to, int weight) {
    int u = addNode(from);
    int v = addNode(to);
    adj_[u].push_back({v, weight});
    if (!directed_) adj_[v].push_back({u, weight});
}

int Graph::nodeId(const std::string& name) const {
    return idOf_.at(name);
}

std::optional<int> Graph::nodeIdOpt(const std::string& name) const {
    return idOf_.get(name);
}

const std::string& Graph::nodeName(int id) const {
    if (id < 0 || id >= (int)nameOf_.size()) throw std::out_of_range("bad node id");
    return nameOf_[id];
}

