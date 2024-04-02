#include <GL/glew.h>
#include "Engine.h"
#include "imgui.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#include <iostream>

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
        100.0f,                      // Far clipping plane
        0.2f
    );
    imguiManager = new ImGuiManager(mainWindow->getGLFWwindow());

    firstMouse = true;
    lastX = 1500 / 2.0;
    lastY = 900 / 2.0;


    glfwSetWindowUserPointer(mainWindow->getGLFWwindow(), this);

    glfwSetCursorPosCallback(mainWindow->getGLFWwindow(), [](GLFWwindow* window, double xpos, double ypos) {
        auto engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
        engine->processMouseMovement(xpos, ypos);
    });

    glfwSetMouseButtonCallback(mainWindow->getGLFWwindow(), [](GLFWwindow* window, int button, int action, int mods) {
        auto engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
        engine->processMouseButton(button, action, mods);
    });

    glfwSetInputMode(mainWindow->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, mainWindow->getWidth(), mainWindow->getHeight());

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shaderProgram = new ShaderCompiler("vertex_shader.glsl", "fragment_shader.glsl");

}


void Engine::run() {
    lastFrameTime = glfwGetTime();

    while (!mainWindow->shouldClose()) {
        const double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f); 
        glm::mat4 view = mainCamera->getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1500.0f / 900.0f, 0.1f, 100.0f);


        shaderProgram->use();
        shaderProgram->setMat4("model", model);
        shaderProgram->setMat4("view", view);
        shaderProgram->setMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        processInput();

        renderImGui();

        mainWindow->run();
    }
}

void Engine::renderImGui() {
    if (!imguiEnabled) return;

    imguiManager->Begin();

    ImGui::Begin("Engine Debug Info");

    ImGui::Text("Window Size: %dx%d", 1500, 900);
    ImGui::Separator();
    ImGui::Text("Camera Position: X: %.2f Y: %.2f Z: %.2f", mainCamera->getPosition().x, mainCamera->getPosition().y, mainCamera->getPosition().z);
    ImGui::Text("Camera Front: X: %.2f Y: %.2f Z: %.2f", mainCamera->getFront().x, mainCamera->getFront().y, mainCamera->getFront().z);
    ImGui::Text("Camera Up: X: %.2f Y: %.2f Z: %.2f", mainCamera->getUp().x, mainCamera->getUp().y, mainCamera->getUp().z);
    float sensitivity = mainCamera->getSensitivity();
    if (ImGui::SliderFloat("Camera Sensitivity", &sensitivity, 0.1f, 5.0f)) {
        mainCamera->setSensitivity(sensitivity);
    }
    ImGui::Text("Yaw: %.2f, Pitch: %.2f", mainCamera->getYaw(), mainCamera->getPitch());
    ImGui::Separator();
    ImGui::Text("Frame Time: %.4f seconds (%.2f ms)", deltaTime, deltaTime * 1000.0);
    ImGui::Text("FPS: %.2f", 1.0f / deltaTime);

    ImGui::End();

    imguiManager->End();
}


void Engine::processInput() {
    GLFWwindow* window = mainWindow->getGLFWwindow();
    static bool escPressedLastFrame = false;
    static bool tabPressedLastFrame = false;

    int escState = glfwGetKey(window, GLFW_KEY_ESCAPE);
    if (escState == GLFW_PRESS && !escPressedLastFrame) {
        toggleCursor();
    }
    escPressedLastFrame = (escState == GLFW_PRESS);

    int tabState = glfwGetKey(window, GLFW_KEY_TAB);
    if (tabState == GLFW_PRESS && !tabPressedLastFrame) {
        toggleImGui();
    }
    tabPressedLastFrame = (tabState == GLFW_PRESS);

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

void Engine::processMouseMovement(double xpos, double ypos) {
    if (cursorEnabled) return;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    mainCamera->processCamMouseMovement(xoffset, yoffset, true);
}

void Engine::processMouseButton(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Handle left mouse button press event
    }
}

void Engine::toggleCursor() {
    cursorEnabled = !cursorEnabled;
    glfwSetInputMode(mainWindow->getGLFWwindow(), GLFW_CURSOR, cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Engine::toggleImGui() {
    imguiEnabled = !imguiEnabled;
}

Engine::~Engine() {
	delete mainWindow;
    delete mainCamera;
    delete imguiManager;
    delete shaderProgram;
}