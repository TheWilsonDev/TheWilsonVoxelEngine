#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include "FastNoiseLite.h"
#include "Voxel.h"
#include "ShaderCompiler.h"
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

class Chunk {
public:
    Chunk();
    ~Chunk();

    void render(ShaderCompiler* shaderProgram);
    void generateChunkAt(const glm::ivec2& chunkCoord, bool centered);
    size_t getVoxelCount() const { return voxels.size(); }
    glm::ivec3 getDirection(int faceIndex);

private:
    struct IVec3Hash {
        size_t operator()(const glm::ivec3& vec) const {
            std::hash<int> hasher;
            size_t seed = 0;
            seed ^= hasher(vec.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= hasher(vec.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= hasher(vec.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
    std::unordered_map<glm::ivec3, bool, IVec3Hash> voxelMap;
    std::vector<std::unique_ptr<Voxel>> voxels;
    FastNoiseLite noise;
    GLuint VAO, VBO;
    int chunkSize;
    void setupMesh();
};
