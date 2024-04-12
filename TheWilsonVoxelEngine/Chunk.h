#pragma once
#include <GL/glew.h>
#include "Voxel.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "ShaderCompiler.h"
#include "FastNoiseLite.h"

struct IVec3Hash {
    size_t operator()(const glm::ivec3& v) const {
        return std::hash<int>()(v.x) ^ std::hash<int>()(v.y) ^ std::hash<int>()(v.z);
    }
};

class Chunk {
public:
    std::vector<Voxel> voxels;
    std::vector<float> vertices;
    std::unordered_map<glm::ivec3, bool, IVec3Hash> voxelMap;
    unsigned int VAO, VBO;
    FastNoiseLite noise;

    Chunk();
    ~Chunk();

    void setupMesh();

    bool isVoxelPresent(const glm::ivec3& pos) const;


    void generateChunkAt(const glm::ivec2& chunkCoord, bool centered);
    void addCubeVertices(const glm::ivec3& position);
    void render(ShaderCompiler* shaderProgram);

    std::vector<glm::vec3> cubeVertices = {
        glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.5f, -0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(-0.5f,  0.5f,  0.5f)
    };

    std::vector<int> indices = {
        0, 1, 2, 0, 2, 3,
        1, 5, 6, 1, 6, 2,
        7, 6, 5, 7, 5, 4,
        0, 3, 7, 0, 7, 4,
        3, 2, 6, 3, 6, 7,
        0, 4, 5, 0, 5, 1
    };
};
