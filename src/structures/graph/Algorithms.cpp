#include "structures/graph/Algorithms.hpp"
#include <queue>
#include <limits>

namespace Algorithms {

std::vector<int> bfs(const Graph& g, int start) {
    std::vector<int> order;
    std::vector<bool> vis(g.V(), false);
    std::queue<int> q;

    vis[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (auto e : g.neighbors(u)) {
            if (!vis[e.to]) {
                vis[e.to] = true;
                q.push(e.to);
            }
        }
    }
    return order;
}

static bool dfsCycle(const Graph& g, int u, std::vector<int>& state) {
    state[u] = 1;
    for (auto e : g.neighbors(u)) {
        int v = e.to;
        if (state[v] == 1) return true;
        if (state[v] == 0 && dfsCycle(g, v, state)) return true;
    }
    state[u] = 2;
    return false;
}

bool hasDirectedCycle(const Graph& g) {
    std::vector<int> state(g.V(), 0);
    for (int u=0; u<g.V(); u++) {
        if (state[u] == 0 && dfsCycle(g, u, state)) return true;
    }
    return false;
}

struct HeapItem { int d; int node; };
struct MinCmp { bool operator()(const HeapItem& a, const HeapItem& b) const { return a.d > b.d; } };

PathResult dijkstra(const Graph& g, int src, int dst) {
    const int INF = std::numeric_limits<int>::max() / 4;
    std::vector<int> dist(g.V(), INF);
    std::vector<int> parent(g.V(), -1);

    std::priority_queue<HeapItem, std::vector<HeapItem>, MinCmp> pq;
    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();
        int d = cur.d, u = cur.node;
        if (d != dist[u]) continue;
        if (u == dst) break;

        for (auto e : g.neighbors(u)) {
            int v = e.to;
            int nd = dist[u] + e.w;
            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                pq.push({nd, v});
            }
        }
    }

    PathResult res;
    if (dist[dst] == INF) return res;
    res.reachable = true;
    res.distance = dist[dst];

    // reconstruct
    std::vector<int> rev;
    for (int cur=dst; cur!=-1; cur=parent[cur]) rev.push_back(cur);
    res.path.assign(rev.rbegin(), rev.rend());
    return res;
}

MSTResult primMST(const Graph& g, int start) {
    const int INF = std::numeric_limits<int>::max() / 4;
    std::vector<int> key(g.V(), INF);
    std::vector<int> parent(g.V(), -1);
    std::vector<bool> in(g.V(), false);

    std::priority_queue<HeapItem, std::vector<HeapItem>, MinCmp> pq;
    key[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();
        int u = cur.node;
        if (in[u]) continue;
        in[u] = true;

        for (auto e : g.neighbors(u)) {
            int v = e.to;
            if (!in[v] && e.w < key[v]) {
                key[v] = e.w;
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    MSTResult res;
    res.parent = parent;
    long long sum = 0;
    for (int i=0;i<g.V();i++) {
        if (i == start) continue;
        if (key[i] != INF) sum += key[i];
    }
    res.totalCost = (int)sum;
    return res;
}

}

