#include "Voxel.h"
#include <gtc/matrix_transform.hpp>

Voxel::Voxel() : VBO(0), VAO(0), position(0.0f, 0.0f, 0.0f) {
    // Voxel Constructor
}

Voxel::~Voxel() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Voxel::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Voxel::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Voxel::draw(ShaderCompiler& shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    shader.use();
    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
