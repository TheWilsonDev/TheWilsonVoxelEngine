#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include "ShaderCompiler.h"
#include <vector>
#include <unordered_map>

enum class VoxelFace {
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

class Voxel
{
public:
    Voxel();
    ~Voxel();

    void init();
	void draw(ShaderCompiler& shader);
	void setPosition(const glm::vec3& pos);
	glm::vec3 getPosition() { return position; }
	std::vector<float> getVertices() { return vertices; }
	void setFaceExposure(VoxelFace face, bool exposed);
	bool isFaceExposed(VoxelFace face) const;
	void generateVerticesBasedOnExposure();
private:
	std::unordered_map<VoxelFace, bool> exposedFaces;
	unsigned int VBO = 0, VAO = 0;
	glm::vec3 position;
	int vertexCount = 0;
	void addFaceVertices(const std::vector<float>& faceVertices, std::vector<float>& vertices);
	const std::vector<float> vertices = {
		// Positions
		-0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f, // Back face
		-0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, // Back face

		-0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, // Front face
		-0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, // Front face

		-0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f, // Top face
		-0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, // Top face

		-0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f, // Bottom face
		-0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, // Bottom face

		 0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, // Right face
		 0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f, // Right face

		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, // Left face
		-0.5f, -0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f  // Left face
	};

};

