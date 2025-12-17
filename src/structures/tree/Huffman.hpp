#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <optional>

// Minimal Huffman coding for learning:
// - compress(std::string) -> bytes + frequency header
// - decompress(bytes) -> original string

struct HuffmanBlob {
    std::vector<std::uint32_t> freq;  // size 256
    std::vector<std::uint8_t> data;   // bit-packed stream (with bitLen)
    std::uint32_t bitLen{0};
};

class Huffman {
public:
    static HuffmanBlob compress(const std::string& input);
    static std::string decompress(const HuffmanBlob& blob);

    static bool saveToFile(const std::string& path, const HuffmanBlob& blob);
    static std::optional<HuffmanBlob> loadFromFile(const std::string& path);

private:
    struct Node {
        int ch; // -1 internal, 0..255 leaf
        std::uint64_t f;
        Node* left{nullptr};
        Node* right{nullptr};
    };

    static Node* buildTree(const std::vector<std::uint32_t>& freq);
    static void buildCodes(Node* n, std::vector<std::vector<bool>>& codes, std::vector<bool>& cur);
    static void destroy(Node* n);

    static void writeBits(const std::vector<bool>& bits, std::vector<std::uint8_t>& out, std::uint32_t& bitLen);
    static bool readBit(const std::vector<std::uint8_t>& in, std::uint32_t i);
};

