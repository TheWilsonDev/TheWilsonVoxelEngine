#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include "FastNoiseLite.h"
#include "Voxel.h"
#include <vector>
#include <memory>


class Chunk
{
public:
	Chunk();
	~Chunk();

	void render(ShaderCompiler* shaderProgram);
	void generateChunkAt(const glm::ivec2& chunkCoord, bool centered);
	int getInstanceCount() { return instancePositions.size(); }

private:
	Voxel* voxel;
	FastNoiseLite noise;
	std::vector<float> voxelVertices;
	std::vector<glm::vec3> instancePositions;
	GLuint instanceVBO;
	GLuint VAO, VBO;
	int chunkSize;
	void setupMesh();
};

