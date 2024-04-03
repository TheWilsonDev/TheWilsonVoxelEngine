#pragma once
#include "ShaderCompiler.h"
#include "Window.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Voxel.h"
#include "Chunk.h"
#include "FastNoiseLite.h"
#include <vector>
#include <vec2.hpp>
#include "utils.h"
#include <unordered_map>
#include <utility>
#include <memory>

class Engine
{
public:
	Engine();
	~Engine();

	void run();
	void processInput();
	void processMouseMovement(double xpos, double ypos);
	void processMouseButton(int button, int action, int mods);
	void processScroll(double xoffset, double yoffset);
	glm::ivec2 getPlayerChunkCoordinates();
	std::vector<glm::ivec2> calculateRequiredChunksAround(const glm::ivec2& chunkCoord, int radius);
	void renderImGui();
	void toggleCursor();
	void toggleImGui();
	void toggleWireFrame();
	void createVoxel(int x, int y, int z);
	void updateChunksBasedOnPlayerPosition();

private:
	Window* mainWindow;
	Camera* mainCamera;
	ImGuiManager* imguiManager;
	ShaderCompiler* shaderProgram;
	Chunk* chunkTest;
	std::vector<std::unique_ptr<Voxel>> terrainVoxels;
	FastNoiseLite noise;
	std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, IVec2Hash, IVec2Equal> activeChunks;
	double lastX, lastY;
	bool firstMouse;
	double deltaTime = 0.0;
	double lastFrameTime = 0.0;
	float fpsHistory[100] = {};
	int currentFrame = 0;
	int historySize = sizeof(fpsHistory) / sizeof(fpsHistory[0]);
	bool cursorEnabled = false;
	bool imguiEnabled = true;
	bool wireframeEnabled = false;
};

