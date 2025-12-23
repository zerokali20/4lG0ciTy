cmake_minimum_required(VERSION 3.16)
project(AlgoCity LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(algocity
    src/main.cpp
    src/modules/CitizenDB.cpp
    src/modules/CityMap.cpp
    src/modules/Emergency.cpp
    src/modules/FileIO.cpp

    src/structures/tree/BST.cpp
    src/structures/tree/AVLTree.cpp
    src/structures/tree/Heap.cpp
    src/structures/tree/Huffman.cpp

    src/structures/graph/Graph.cpp
    src/structures/graph/Algorithms.cpp
    src/structures/graph/TopoSort.cpp

    src/structures/hash/HashTable.cpp
    src/structures/hash/HashUtils.cpp
)

target_include_directories(algocity PRIVATE src)

