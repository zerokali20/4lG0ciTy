#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include "modules/CitizenDB.hpp"
#include "modules/CityMap.hpp"
#include "modules/Emergency.hpp"
#include "modules/FileIO.hpp"
#include "structures/graph/TopoSort.hpp"

static void flushLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int readInt(const std::string& prompt) {
    int x;
    std::cout << prompt;
    while (!(std::cin >> x)) {
        std::cin.clear();
        flushLine();
        std::cout << "Enter a number: ";
    }
    flushLine();
    return x;
}

static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

static void runTopoFromFile(const std::string& path) {
    Graph tasks(true);
    std::ifstream f(path);
    if (!f) { std::cout << "Failed to open: " << path << "\n"; return; }

    std::string line;
    int edges = 0;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string pre, task;
        if (!std::getline(ss, pre, ';')) continue;
        if (!std::getline(ss, task, ';')) continue;
        tasks.addEdge(pre, task, 1);
        edges++;
    }
    std::cout << "Loaded tasks: " << edges << " deps, " << tasks.V() << " tasks.\n";

    auto res = TopoSort::kahn(tasks);
    if (!res.isDAG) {
        std::cout << "Cannot topo-sort: cycle detected.\n";
        return;
    }
    std::cout << "Valid construction order:\n";
    for (int id : res.order) std::cout << " - " << tasks.nodeName(id) << "\n";
}

int main() {
    CitizenDB citizens;
    CityMap city;
    EmergencySystem emergency;

    while (true) {
        std::cout << "\n=== AlgoCity ===\n"
                  << "1) Citizen Registry\n"
                  << "2) City Map\n"
                  << "3) Emergency Response\n"
                  << "4) Construction Planner (Topo Sort)\n"
                  << "5) Save/Load demo (Huffman)\n"
                  << "0) Exit\n";

        int choice = readInt("Choose: ");
        if (choice == 0) break;

        if (choice == 1) {
            std::cout << "\nCitizen Registry\n"
                      << "1) Load from data/residents.txt\n"
                      << "2) Toggle BST/AVL\n"
                      << "3) Add citizen\n"
                      << "4) Find citizen\n"
                      << "5) Remove citizen\n"
                      << "6) Stats\n";
            int c = readInt("Choose: ");
            if (c == 1) citizens.loadFromFile("data/residents.txt");
            else if (c == 2) {
                auto m = citizens.mode();
                citizens.setMode(m == CitizenDB::Mode::BSTMode ? CitizenDB::Mode::AVLMode : CitizenDB::Mode::BSTMode);
                citizens.printStats();
            } else if (c == 3) {
                Citizen z;
                z.id = readInt("ID: ");
                z.name = readLine("Name: ");
                z.age = readInt("Age: ");
                citizens.insert(z);
            } else if (c == 4) {
                int id = readInt("ID to find: ");
                citizens.findAndPrint(id);
            } else if (c == 5) {
                int id = readInt("ID to remove: ");
                std::cout << (citizens.erase(id) ? "Removed.\n" : "Not found.\n");
            } else if (c == 6) citizens.printStats();
        }

        else if (choice == 2) {
            std::cout << "\nCity Map\n"
                      << "1) Load roads from data/city_layout.txt\n"
                      << "2) Add road\n"
                      << "3) BFS from location\n"
                      << "4) Shortest path (Dijkstra)\n"
                      << "5) Optimize power grid (Prim MST)\n";
            int c = readInt("Choose: ");
            if (c == 1) city.loadRoads("data/city_layout.txt");
            else if (c == 2) {
                std::string a = readLine("From: ");
                std::string b = readLine("To: ");
                int w = readInt("Distance: ");
                city.addRoad(a, b, w);
            } else if (c == 3) {
                std::string s = readLine("Start location: ");
                city.bfsFrom(s);
            } else if (c == 4) {
                std::string a = readLine("From: ");
                std::string b = readLine("To: ");
                city.shortestPath(a, b);
            } else if (c == 5) {
                std::string s = readLine("Start (power plant): ");
                city.optimizePowerGrid(s);
            }
        }

        else if (choice == 3) {
            std::cout << "\nEmergency Response\n"
                      << "1) Report emergency\n"
                      << "2) Dispatch next\n";
            int c = readInt("Choose: ");
            if (c == 1) {
                EmergencyEvent e;
                e.severity = readInt("Severity (1-100): ");
                e.type = readLine("Type: ");
                e.location = readLine("Location: ");
                e.details = readLine("Details: ");
                emergency.report(e);
            } else if (c == 2) {
                emergency.dispatchNext();
            }
        }

        else if (choice == 4) {
            runTopoFromFile("data/construction_tasks.txt");
        }

        else if (choice == 5) {
            std::cout << "\nHuffman Save/Load demo\n"
                      << "1) Save sample text (compressed)\n"
                      << "2) Load sample text (decompress)\n";
            int c = readInt("Choose: ");
            if (c == 1) {
                std::string text =
                    "AlgoCity save state demo:\n"
                    "Nodes: CityHall, Airport, Hospital, GrandHotel\n"
                    "Roads: CityHall-GrandHotel(5), GrandHotel-Airport(12)\n";
                bool ok = FileIO::saveCompressedText("data/city_state.huf", text);
                std::cout << (ok ? "Saved to data/city_state.huf\n" : "Save failed.\n");
            } else if (c == 2) {
                std::string text = FileIO::loadCompressedText("data/city_state.huf");
                if (text.empty()) std::cout << "Load failed.\n";
                else std::cout << "Decompressed text:\n" << text << "\n";
            }
        }
    }

    std::cout << "Bye!\n";
    return 0;
}

