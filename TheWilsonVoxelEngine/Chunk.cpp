#include "Chunk.h"
#include "FastNoiseLite.h"
#include <iostream>
#include <cmath>

Chunk::Chunk() : chunkSize(16) {
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.05f);
    setupMesh();
}

Chunk::~Chunk() {
    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Chunk::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Chunk::generateChunkAt(const glm::ivec2& chunkCoord, bool centered) {
    voxels.clear();
    voxelMap.clear();

    int startX = chunkCoord.x * chunkSize;
    int startZ = chunkCoord.y * chunkSize;
    if (centered) {
        startX = 0;
        startZ = 0;
    }

    for (int x = 0; x < chunkSize; ++x) {
        for (int z = 0; z < chunkSize; ++z) {
            int worldX = startX + x;
            int worldZ = startZ + z;

            float height = noise.GetNoise(static_cast<float>(worldX), static_cast<float>(worldZ)) * 40.0f;
            int maxHeight = std::round(height);

            for (int worldY = -15; worldY <= maxHeight; ++worldY) {
                glm::ivec3 pos(worldX, worldY, worldZ);
                voxelMap[pos] = true;
            }
        }
    }

    for (const auto& entry : voxelMap) {
        const glm::ivec3& pos = entry.first;
        auto voxel = std::make_unique<Voxel>();
        voxel->setPosition(glm::vec3(pos.x, pos.y, pos.z));

        for (int i = 0; i < 6; ++i) {
            glm::ivec3 dir = getDirection(i);
            glm::ivec3 neighborPos = pos + dir;

            if (voxelMap.find(neighborPos) == voxelMap.end()) {
                voxel->setFaceExposure(static_cast<VoxelFace>(i), true);
            }
        }

        voxel->generateVerticesBasedOnExposure();
        voxels.push_back(std::move(voxel));
    }
}

glm::ivec3 Chunk::getDirection(int faceIndex) {
    switch (faceIndex) {
    case 0: return glm::ivec3(0, 0, 1);
    case 1: return glm::ivec3(0, 0, -1);
    case 2: return glm::ivec3(-1, 0, 0);
    case 3: return glm::ivec3(1, 0, 0);
    case 4: return glm::ivec3(0, 1, 0);
    case 5: return glm::ivec3(0, -1, 0);
    default: return glm::ivec3(0);
    }
}

void Chunk::render(ShaderCompiler* shaderProgram) {
    shaderProgram->use();
    for (auto& voxel : voxels) {
        voxel->draw(*shaderProgram);
    }
}