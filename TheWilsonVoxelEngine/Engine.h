#pragma once
#include "Window.h"
#include "Camera.h"

class Engine
{
public:
	Engine();
	~Engine();

	void run();
	void processInput();
private:
	Window* mainWindow;
	Camera* mainCamera;
	double deltaTime;
	double lastFrameTime;
};

