#pragma once
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(int width, int height, const char* title);
	~Window();

	void clearContext();
	void run();
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	GLFWwindow* getGLFWwindow() const { return window; }
	bool shouldClose();
	int getWidth() const;
	int getHeight() const;
private:
	GLFWwindow* window;
	int width, height;
};

