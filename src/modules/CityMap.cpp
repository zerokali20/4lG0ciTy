#include "modules/CityMap.hpp"
#include "structures/graph/Algorithms.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

CityMap::CityMap() : roads_(false) {}

void CityMap::loadRoads(const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        std::cout << "Failed to open: " << path << "\n";
        return;
    }
    std::string line;
    int edges = 0;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string a,b,sw;
        if (!std::getline(ss,a,';')) continue;
        if (!std::getline(ss,b,';')) continue;
        if (!std::getline(ss,sw,';')) continue;
        addRoad(a,b,std::stoi(sw));
        edges++;
    }
    std::cout << "Loaded roads: " << edges << " edges, " << roads_.V() << " nodes.\n";
}

void CityMap::addRoad(const std::string& a, const std::string& b, int w) {
    roads_.addEdge(a,b,w);
}

void CityMap::bfsFrom(const std::string& start) const {
    auto sid = roads_.nodeIdOpt(start);
    if (!sid) { std::cout << "Unknown location.\n"; return; }
    auto order = Algorithms::bfs(roads_, *sid);
    std::cout << "BFS order from " << start << ":\n";
    for (int id : order) std::cout << " - " << roads_.nodeName(id) << "\n";
}

void CityMap::shortestPath(const std::string& a, const std::string& b) const {
    auto ia = roads_.nodeIdOpt(a);
    auto ib = roads_.nodeIdOpt(b);
    if (!ia || !ib) { std::cout << "Unknown location(s).\n"; return; }

    auto res = Algorithms::dijkstra(roads_, *ia, *ib);
    if (!res.reachable) {
        std::cout << "No route found.\n";
        return;
    }
    std::cout << "Shortest distance: " << res.distance << "\nPath:\n";
    for (std::size_t i=0;i<res.path.size();i++) {
        std::cout << roads_.nodeName(res.path[i]);
        if (i+1<res.path.size()) std::cout << " -> ";
    }
    std::cout << "\n";
}

void CityMap::optimizePowerGrid(const std::string& start) const {
    auto s = roads_.nodeIdOpt(start);
    if (!s) { std::cout << "Unknown start location.\n"; return; }

    auto mst = Algorithms::primMST(roads_, *s);
    std::cout << "MST total cost (wire length): " << mst.totalCost << "\nEdges:\n";
    for (int v=0; v<roads_.V(); v++) {
        int p = mst.parent[v];
        if (p == -1) continue;
        std::cout << " - " << roads_.nodeName(p) << " <-> " << roads_.nodeName(v) << "\n";
    }
}

