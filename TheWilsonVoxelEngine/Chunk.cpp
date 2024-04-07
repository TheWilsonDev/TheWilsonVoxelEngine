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

            for (int worldY = 0; worldY <= maxHeight; ++worldY) {
                glm::ivec3 pos(worldX, worldY, worldZ);
                voxelMap[pos] = true;
            }
        }
    }

    /*unsigned int vertexCount = 0;

    for (const auto& entry : voxelMap) {
        const glm::ivec3& pos = entry.first;

        for (int i = 0; i < 6; ++i) {
            glm::ivec3 dir = getDirection(i);
            glm::ivec3 neighborPos = pos + dir;

            if (voxelMap.find(neighborPos) == voxelMap.end()) {
                addVoxelFaceVertices(vertices, indices, pos, i, vertexCount);
                vertexCount += 4; 
            }
        }
    }

    combinedMesh.createMesh(vertices, indices);*/

    for (const auto& entry : voxelMap) {
        const glm::ivec3& pos = entry.first;

        for (int i = 0; i < 6; ++i) {
            glm::ivec3 dir = getDirection(i);
            glm::ivec3 neighborPos = pos + dir;

            if (voxelMap.find(neighborPos) == voxelMap.end()) {
                addVoxelFaceVertices(vertices, pos, i);
            }
        }
    }

    combinedMesh.createMesh(vertices);
}

//void Chunk::addVoxelFaceVertices(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, const glm::ivec3& pos, int face, unsigned int& vertexCount) {
//    glm::vec3 cubeVertices[8] = {
//        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
//        glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
//        glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f),
//        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)
//    };
//
//    unsigned int faceIndices[6][4] = {
//        {3, 2, 1, 0}, // Top
//        {4, 5, 6, 7}, // Bottom
//        {7, 3, 0, 4}, // Left
//        {1, 2, 6, 5}, // Right
//        {7, 6, 2, 3}, // Front
//        {0, 1, 5, 4}  // Back
//    };
//
//    for (int j = 0; j < 4; j++) {
//        unsigned int vertexIndex = faceIndices[face][j];
//        vertices.push_back(glm::vec3(pos) + cubeVertices[vertexIndex]);
//    }
//
//    indices.push_back(vertexCount);
//    indices.push_back(vertexCount + 1);
//    indices.push_back(vertexCount + 2);
//    indices.push_back(vertexCount);
//    indices.push_back(vertexCount + 2);
//    indices.push_back(vertexCount + 3);
//
//    vertexCount += 4;
//}

void Chunk::addVoxelFaceVertices(std::vector<glm::vec3>& vertices, const glm::ivec3& pos, int face) {
    glm::vec3 cubeVertices[8] = {
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)
    };

    // Define vertices for each face, adjusted for counter-clockwise winding order
    unsigned int faceIndices[6][6] = {
        // Each face defined by two triangles, thus 6 vertices per face
        {3, 2, 1, 3, 1, 0}, // Top
        {4, 5, 6, 4, 6, 7}, // Bottom
        {7, 6, 2, 7, 2, 3}, // Left
        {0, 1, 5, 0, 5, 4}, // Right
        {7, 3, 0, 7, 0, 4}, // Front
        {1, 2, 6, 1, 6, 5}  // Back
    };

    for (int i = 0; i < 6; i++) { // Now adding 6 vertices for two triangles per face
        vertices.push_back(glm::vec3(pos) + cubeVertices[faceIndices[face][i]]);
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
    combinedMesh.renderMesh();
}