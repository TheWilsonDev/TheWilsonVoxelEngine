#include "Engine.h"

Engine::Engine() {
	// Engine Constructor
    mainWindow = new Window(1500, 900, "TheWilsonVoxelEngine");
    deltaTime = 0.0;
    lastFrameTime = 0.0;
    mainCamera = new Camera(
        glm::vec3(0.0f, 0.0f, 3.0f), // Position
        glm::vec3(0.0f, 1.0f, 0.0f), // Up
        -90.0f,                      // Yaw
        0.0f,                        // Pitch
        45.0f,                       // FOV
        1500.0f / 900.0f,            // Aspect Ratio
        0.1f,                        // Near clipping plane
        100.0f                       // Far clipping plane
    );
}


void Engine::run() {
    lastFrameTime = glfwGetTime();

    while (!mainWindow->shouldClose()) {
        const double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        processInput();

        mainWindow->run();
    }
}

void Engine::processInput() {
    GLFWwindow* window = mainWindow->getGLFWwindow();

    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mainCamera->setPosition(mainCamera->getPosition() + cameraSpeed * mainCamera->getFront());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mainCamera->setPosition(mainCamera->getPosition() - cameraSpeed * mainCamera->getFront());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mainCamera->setPosition(mainCamera->getPosition() - glm::normalize(glm::cross(mainCamera->getFront(), mainCamera->getUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mainCamera->setPosition(mainCamera->getPosition() + glm::normalize(glm::cross(mainCamera->getFront(), mainCamera->getUp())) * cameraSpeed);
}

Engine::~Engine() {
	delete mainWindow;
}