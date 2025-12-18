#pragma once
#include <string>
#include <cstdint>

namespace HashUtils {
    // FNV-1a 64-bit for strings (stable, simple, good distribution for learning)
    std::uint64_t fnv1a64(const std::string& s);

    // Next power of two >= n (used for table sizing)
    std::size_t nextPow2(std::size_t n);
}

