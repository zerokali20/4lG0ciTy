#include "structures/graph/TopoSort.hpp"
#include <queue>

namespace TopoSort {

Result kahn(const Graph& g) {
    Result res;
    std::vector<int> indeg(g.V(), 0);
    for (int u=0; u<g.V(); u++) {
        for (auto e : g.neighbors(u)) indeg[e.to]++;
    }

    std::queue<int> q;
    for (int i=0;i<g.V();i++) if (indeg[i]==0) q.push(i);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        res.order.push_back(u);
        for (auto e : g.neighbors(u)) {
            if (--indeg[e.to] == 0) q.push(e.to);
        }
    }

    res.isDAG = ((int)res.order.size() == g.V());
    return res;
}

}

