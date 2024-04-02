#pragma once

#include <GLFW/glfw3.h>

class ImGuiManager {
public:
    ImGuiManager(GLFWwindow* window);
    ~ImGuiManager();

    void Begin();
    void End();

private:
    GLFWwindow* window;
};