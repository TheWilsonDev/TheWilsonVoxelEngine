#pragma once
#include <glm.hpp>
#include <unordered_map>

struct IVec2Hash {
    size_t operator()(const glm::ivec2& vec) const {
        auto hash1 = std::hash<int>{}(vec.x);
        auto hash2 = std::hash<int>{}(vec.y);
        return hash1 ^ (hash2 << 1);
    }
};

struct IVec2Equal {
    bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
        return a.x == b.x && a.y == b.y;
    }
};
