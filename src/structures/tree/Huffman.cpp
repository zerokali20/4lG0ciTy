#include "structures/tree/Huffman.hpp"
#include <queue>
#include <fstream>

static Huffman::Node* makeNode(int ch, std::uint64_t f, Huffman::Node* l=nullptr, Huffman::Node* r=nullptr){
    auto* n = new Huffman::Node();
    n->ch = ch; n->f = f; n->left = l; n->right = r;
    return n;
}

struct CmpNode {
    bool operator()(const Huffman::Node* a, const Huffman::Node* b) const {
        return a->f > b->f;
    }
};

Huffman::Node* Huffman::buildTree(const std::vector<std::uint32_t>& freq) {
    std::priority_queue<Node*, std::vector<Node*>, CmpNode> pq;
    for (int i=0;i<256;i++) {
        if (freq[i] > 0) pq.push(makeNode(i, freq[i]));
    }
    if (pq.empty()) return makeNode(-1, 0);
    if (pq.size()==1) {
        // single symbol: add dummy
        Node* only = pq.top(); pq.pop();
        pq.push(makeNode(-1, only->f, only, makeNode(0,0)));
    }
    while (pq.size() > 1) {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(makeNode(-1, a->f + b->f, a, b));
    }
    return pq.top();
}

void Huffman::buildCodes(Node* n, std::vector<std::vector<bool>>& codes, std::vector<bool>& cur) {
    if (!n) return;
    if (n->ch >= 0) {
        codes[n->ch] = cur;
        return;
    }
    cur.push_back(false);
    buildCodes(n->left, codes, cur);
    cur.pop_back();

    cur.push_back(true);
    buildCodes(n->right, codes, cur);
    cur.pop_back();
}

void Huffman::destroy(Node* n) {
    if (!n) return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}

void Huffman::writeBits(const std::vector<bool>& bits, std::vector<std::uint8_t>& out, std::uint32_t& bitLen) {
    for (bool b : bits) {
        std::uint32_t i = bitLen++;
        std::size_t byte = i / 8;
        std::size_t off  = i % 8;
        if (byte >= out.size()) out.push_back(0);
        if (b) out[byte] |= static_cast<std::uint8_t>(1u << off);
    }
}

bool Huffman::readBit(const std::vector<std::uint8_t>& in, std::uint32_t i) {
    std::size_t byte = i / 8;
    std::size_t off  = i % 8;
    return (in[byte] >> off) & 1u;
}

HuffmanBlob Huffman::compress(const std::string& input) {
    HuffmanBlob blob;
    blob.freq.assign(256, 0);

    for (unsigned char c : input) blob.freq[c]++;

    Node* root = buildTree(blob.freq);
    std::vector<std::vector<bool>> codes(256);
    std::vector<bool> cur;
    buildCodes(root, codes, cur);

    blob.data.clear();
    blob.bitLen = 0;
    for (unsigned char c : input) {
        writeBits(codes[c], blob.data, blob.bitLen);
    }

    destroy(root);
    return blob;
}

std::string Huffman::decompress(const HuffmanBlob& blob) {
    Node* root = buildTree(blob.freq);
    std::string out;
    out.reserve(1024);

    Node* cur = root;
    for (std::uint32_t i=0; i<blob.bitLen; i++) {
        bool b = readBit(blob.data, i);
        cur = b ? cur->right : cur->left;
        if (!cur) break;
        if (cur->ch >= 0) {
            out.push_back(static_cast<char>(cur->ch));
            cur = root;
        }
    }

    destroy(root);
    return out;
}

// File format (simple):
// [256 uint32 freq][uint32 bitLen][uint32 dataBytes][data...]
bool Huffman::saveToFile(const std::string& path, const HuffmanBlob& blob) {
    std::ofstream f(path, std::ios::binary);
    if (!f) return false;

    for (int i=0;i<256;i++) {
        std::uint32_t x = blob.freq[i];
        f.write(reinterpret_cast<const char*>(&x), sizeof(x));
    }
    f.write(reinterpret_cast<const char*>(&blob.bitLen), sizeof(blob.bitLen));
    std::uint32_t n = static_cast<std::uint32_t>(blob.data.size());
    f.write(reinterpret_cast<const char*>(&n), sizeof(n));
    if (n) f.write(reinterpret_cast<const char*>(blob.data.data()), n);
    return true;
}

std::optional<HuffmanBlob> Huffman::loadFromFile(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return std::nullopt;

    HuffmanBlob blob;
    blob.freq.assign(256, 0);
    for (int i=0;i<256;i++) {
        std::uint32_t x=0;
        f.read(reinterpret_cast<char*>(&x), sizeof(x));
        if (!f) return std::nullopt;
        blob.freq[i]=x;
    }
    f.read(reinterpret_cast<char*>(&blob.bitLen), sizeof(blob.bitLen));
    if (!f) return std::nullopt;

    std::uint32_t n=0;
    f.read(reinterpret_cast<char*>(&n), sizeof(n));
    if (!f) return std::nullopt;

    blob.data.resize(n);
    if (n) f.read(reinterpret_cast<char*>(blob.data.data()), n);
    if (!f) return std::nullopt;

    return blob;
}

