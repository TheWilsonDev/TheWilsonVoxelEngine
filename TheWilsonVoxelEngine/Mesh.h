#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm.hpp>

class Mesh {
public:
    Mesh();
    ~Mesh();

    void createMesh(const std::vector<glm::vec3>& vertices);
    void renderMesh();
    void clearMesh();

private:
    GLuint VAO, VBO, EBO;
    GLsizei indexCount;
};
