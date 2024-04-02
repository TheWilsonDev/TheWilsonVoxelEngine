#pragma once
#include "ShaderCompiler.h"
#include "Window.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include <vector>

class Engine
{
public:
	Engine();
	~Engine();

	void run();
	void processInput();
	void processMouseMovement(double xpos, double ypos);
	void processMouseButton(int button, int action, int mods);
	void renderImGui();
	void toggleCursor();
	void toggleImGui();
private:
	Window* mainWindow;
	Camera* mainCamera;
	ImGuiManager* imguiManager;
	ShaderCompiler* shaderProgram;
	double lastX, lastY;
	bool firstMouse;
	double deltaTime;
	double lastFrameTime;
	unsigned int VBO, VAO;
	bool cursorEnabled = false;
	bool imguiEnabled = true;
	const std::vector<float> vertices = {
		// Positions          
		-1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, // Back face
		-1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, // Back face

		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, // Front face
		-1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, // Front face

		-1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, // Top face
		-1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, // Top face

		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, // Bottom face
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, // Bottom face

		 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, // Right face
		 1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, // Right face

		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, // Left face
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f  // Left face
	};
};

