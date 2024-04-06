#include "Voxel.h"
#include <gtc/matrix_transform.hpp>

Voxel::Voxel() : position(0.0f, 0.0f, 0.0f) {

}
Voxel::~Voxel() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Voxel::init() {
    generateVerticesBasedOnExposure();
}

void Voxel::generateVerticesBasedOnExposure() {
    std::vector<float> vertices;

    // Front
    const std::vector<float> frontFace = {
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f
    };

    // Back
    const std::vector<float> backFace = {
        0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, 0.5f,  0.5f, -0.5f
    };

    // Left
    const std::vector<float> leftFace = {
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f
    };

    // Right
    const std::vector<float> rightFace = {
        0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f, 0.5f,  0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,  0.5f,  0.5f, -0.5f, 0.5f,  0.5f,  0.5f
    };

    // Top
    const std::vector<float> topFace = {
        -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f
    };

    // Bottom
    const std::vector<float> bottomFace = {
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f
    };

    // Check each face for exposure and append its vertices if exposed
    if (isFaceExposed(VoxelFace::FRONT)) {
        vertices.insert(vertices.end(), frontFace.begin(), frontFace.end());
    }
    if (isFaceExposed(VoxelFace::BACK)) {
        vertices.insert(vertices.end(), backFace.begin(), backFace.end());
    }
    if (isFaceExposed(VoxelFace::LEFT)) {
        vertices.insert(vertices.end(), leftFace.begin(), leftFace.end());
    }
    if (isFaceExposed(VoxelFace::RIGHT)) {
        vertices.insert(vertices.end(), rightFace.begin(), rightFace.end());
    }
    if (isFaceExposed(VoxelFace::TOP)) {
        vertices.insert(vertices.end(), topFace.begin(), topFace.end());
    }
    if (isFaceExposed(VoxelFace::BOTTOM)) {
        vertices.insert(vertices.end(), bottomFace.begin(), bottomFace.end());
    }

    if (isFaceExposed(VoxelFace::FRONT)) addFaceVertices(frontFace, vertices);
    if (isFaceExposed(VoxelFace::BACK)) addFaceVertices(backFace, vertices);
    if (isFaceExposed(VoxelFace::LEFT)) addFaceVertices(leftFace, vertices);
    if (isFaceExposed(VoxelFace::RIGHT)) addFaceVertices(rightFace, vertices);
    if (isFaceExposed(VoxelFace::TOP)) addFaceVertices(topFace, vertices);
    if (isFaceExposed(VoxelFace::BOTTOM)) addFaceVertices(bottomFace, vertices);

    if (!VAO) glGenVertexArrays(1, &VAO);
    if (!VBO) glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertexCount = vertices.size() / 3;
}

void Voxel::addFaceVertices(const std::vector<float>& faceVertices, std::vector<float>& vertices) {
    vertices.insert(vertices.end(), faceVertices.begin(), faceVertices.end());
}

void Voxel::setFaceExposure(VoxelFace face, bool exposed) {
    exposedFaces[face] = exposed;
}

bool Voxel::isFaceExposed(VoxelFace face) const {
    auto found = exposedFaces.find(face);
    return found != exposedFaces.end() && found->second;
}

void Voxel::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Voxel::draw(ShaderCompiler& shader) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    shader.use();
    shader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}