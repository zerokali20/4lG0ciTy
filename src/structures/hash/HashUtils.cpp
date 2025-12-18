#include "structures/hash/HashUtils.hpp"

namespace HashUtils {
    std::uint64_t fnv1a64(const std::string& s) {
        const std::uint64_t FNV_OFFSET = 14695981039346656037ULL;
        const std::uint64_t FNV_PRIME  = 1099511628211ULL;
        std::uint64_t h = FNV_OFFSET;
        for (unsigned char c : s) {
            h ^= static_cast<std::uint64_t>(c);
            h *= FNV_PRIME;
        }
        return h;
    }

    std::size_t nextPow2(std::size_t n) {
        if (n < 2) return 2;
        std::size_t p = 1;
        while (p < n) p <<= 1;
        return p;
    }
}

