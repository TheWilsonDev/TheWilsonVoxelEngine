#include "Chunk.h"
#include <iostream>
#include <cmath>
#include <unordered_map>


Chunk::Chunk() {
    setupMesh();
}

Chunk::~Chunk() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

bool Chunk::isVoxelPresent(const glm::ivec3& pos) const {
    return voxelMap.find(pos) != voxelMap.end();
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
    vertices.clear();
    voxelMap.clear();

    int startX = chunkCoord.x * 16;
    int startZ = chunkCoord.y * 16;
    if (centered) {
        startX = -8;
        startZ = -8;
    }

    for (int x = 0; x < 16; ++x) {
        for (int z = 0; z < 16; ++z) {
            int worldX = startX + x;
            int worldZ = startZ + z;
            float height = noise.GetNoise(static_cast<float>(worldX), static_cast<float>(worldZ)) * 40.0f;
            int maxHeight = std::round(height);
            for (int y = 0; y <= maxHeight; ++y) {
                glm::ivec3 voxelPos(worldX, y, worldZ);
                voxelMap[voxelPos] = true;
                addCubeVertices(voxelPos);
            }
        }
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Chunk::addCubeVertices(const glm::ivec3& position) {
    static const glm::ivec3 directions[6] = {
        glm::ivec3(0, 0, 1), glm::ivec3(0, 0, -1),
        glm::ivec3(-1, 0, 0), glm::ivec3(1, 0, 0),
        glm::ivec3(0, 1, 0), glm::ivec3(0, -1, 0)
    };

    for (int i = 0; i < 6; i++) {
        glm::ivec3 neighborPos = position + directions[i];
        if (!isVoxelPresent(neighborPos)) {
            for (int j = 0; j < 6; j++) {
                int index = indices[6 * i + j];
                glm::vec3 vertex = cubeVertices[index] + glm::vec3(position);
                vertices.push_back(vertex.x);
                vertices.push_back(vertex.y);
                vertices.push_back(vertex.z);
            }
        }
    }
}

void Chunk::render(ShaderCompiler* shaderProgram) {
    shaderProgram->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    glBindVertexArray(0);
}