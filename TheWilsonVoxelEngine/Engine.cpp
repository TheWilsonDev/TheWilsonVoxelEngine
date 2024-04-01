#include "Engine.h"

Engine::Engine() : mainWindow(new Window(1500, 900, "TheWilsonVoxelEngine")) {
	// Engine Constructor
}


void Engine::run() {
	mainWindow->run();
}

Engine::~Engine() {
	delete mainWindow;
}