#include "Chunk.h"
#include "FastNoiseLite.h"
#include <iostream>


Chunk::Chunk() : chunkSize(16) {
	// Chunk Constructor
    voxel = new Voxel();
    voxelVertices = voxel->getVertices();
    setupMesh();
}

Chunk::~Chunk() {
	//Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &instanceVBO);
}

void Chunk::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &instanceVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, voxelVertices.size() * sizeof(float), voxelVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(1, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Chunk::generateChunkAt(const glm::ivec2& chunkCoord, bool centered) {
    instancePositions.clear();

    
    int startX = chunkCoord.x * chunkSize;
    int startZ = chunkCoord.y * chunkSize;
    if (centered) {
        startX = 0;
        startZ = 0;
    }

    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.05f);

    for (int x = 0; x < chunkSize; ++x) {
        for (int z = 0; z < chunkSize; ++z) {
            int worldX = startX + x;
            int worldZ = startZ + z;

            float height = noise.GetNoise(static_cast<float>(worldX), static_cast<float>(worldZ)) * 40.0f;
            int maxHeight = std::round(height);

            // Generate voxels from the ground up to the maxHeight
            for (int worldY = -40; worldY <= maxHeight; ++worldY) {
                instancePositions.emplace_back(worldX, worldY, worldZ);
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, instancePositions.size() * sizeof(glm::vec3), instancePositions.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Chunk::render(ShaderCompiler* shaderProgram) {
    shaderProgram->use();
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, instancePositions.size());
    glBindVertexArray(0);
}