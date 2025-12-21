#include "modules/FileIO.hpp"
#include <iostream>

bool FileIO::saveCompressedText(const std::string& path, const std::string& text) {
    HuffmanBlob blob = Huffman::compress(text);
    if (!Huffman::saveToFile(path, blob)) return false;
    std::cout << "Saved Huffman blob: bits=" << blob.bitLen
              << ", bytes=" << blob.data.size() << "\n";
    return true;
}

std::string FileIO::loadCompressedText(const std::string& path) {
    auto blob = Huffman::loadFromFile(path);
    if (!blob) return "";
    return Huffman::decompress(*blob);
}

