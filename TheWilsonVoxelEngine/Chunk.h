#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include "FastNoiseLite.h"
#include "Voxel.h"
#include "ShaderCompiler.h"
#include "Mesh.h"
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
    glm::ivec3 getDirection(int faceIndex);


    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    Mesh combinedMesh;

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
    FastNoiseLite noise;
    GLuint VAO, VBO;
    int chunkSize;
    void setupMesh();
    //void addVoxelFaceVertices(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, const glm::ivec3& pos, int face, unsigned int& vertexCount);
    void addVoxelFaceVertices(std::vector<glm::vec3>& vertices, const glm::ivec3& pos, int face);
};
