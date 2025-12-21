#pragma once
#include <string>
#include "structures/graph/Graph.hpp"

class CityMap {
public:
    CityMap();

    void loadRoads(const std::string& path);

    void addRoad(const std::string& a, const std::string& b, int w);
    void bfsFrom(const std::string& start) const;
    void shortestPath(const std::string& a, const std::string& b) const;
    void optimizePowerGrid(const std::string& start) const;

    const Graph& graph() const { return roads_; }

private:
    Graph roads_; // undirected weighted
};

