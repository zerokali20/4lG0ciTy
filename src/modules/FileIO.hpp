#pragma once
#include <string>
#include "structures/tree/Huffman.hpp"

// Demonstrates Huffman compression when saving a "city state" text blob.
class FileIO {
public:
    static bool saveCompressedText(const std::string& path, const std::string& text);
    static std::string loadCompressedText(const std::string& path);
};

