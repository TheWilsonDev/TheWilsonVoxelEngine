#pragma once
#include "Window.h"

class Engine
{
public:
	Engine();
	~Engine();

	void init();
	void run();
private:
	Window* mainWindow;
};

