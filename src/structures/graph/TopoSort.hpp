#pragma once
#include <vector>
#include "structures/graph/Graph.hpp"

namespace TopoSort {
    struct Result {
        bool isDAG{false};
        std::vector<int> order;
    };

    Result kahn(const Graph& g);
}

