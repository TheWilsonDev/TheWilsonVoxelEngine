#pragma once
#include "ShaderCompiler.h"
#include "Window.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Voxel.h"
#include "Chunk.h"
#include "FastNoiseLite.h"
#include <vector>
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
	void renderImGui();
	void toggleCursor();
	void toggleImGui();
	void toggleWireFrame();
	void generateTerrain();
	void createVoxel(int x, int y, int z);
private:
	Window* mainWindow;
	Camera* mainCamera;
	ImGuiManager* imguiManager;
	ShaderCompiler* shaderProgram;
	Chunk* chunkTest;
	std::vector<std::unique_ptr<Voxel>> terrainVoxels;
	FastNoiseLite noise;
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

