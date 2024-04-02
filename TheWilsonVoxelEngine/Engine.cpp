#include <GL/glew.h>
#include "Engine.h"
#include "imgui.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#include <iostream>

Engine::Engine() {
	// Engine Constructor
    mainWindow = new Window(1500, 900, "TheWilsonVoxelEngine");
    mainCamera = new Camera(
        glm::vec3(0.0f, 1.0f, 3.0f), // Position
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

    shaderProgram = new ShaderCompiler("vertex_shader.glsl", "fragment_shader.glsl");

    generateTerrain();
}

void Engine::generateTerrain() {
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.01f);

    int terrainWidth = 16;
    int terrainDepth = 16;
    float heightMultiplier = 20.0f;

    for (int x = 0; x < terrainWidth; ++x) {
        for (int z = 0; z < terrainDepth; ++z) {
            float height = noise.GetNoise((float)x, (float)z) * heightMultiplier;
            int y = std::round(height);
            createVoxel(x, y, z);
        }
    }

    //std::cout << "Terrain Voxels Count: " << terrainVoxels.size() << std::endl;
}

void Engine::createVoxel(int x, int y, int z) {
    auto voxel = std::make_unique<Voxel>();
    voxel->setPosition(glm::vec3(x, y, z));
    voxel->init();
    terrainVoxels.push_back(std::move(voxel));
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

        processInput();

        for (auto& voxel : terrainVoxels) {
            voxel->draw(*shaderProgram);
        }

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

    float currentFPS = 1.0f / deltaTime;
    fpsHistory[currentFrame % historySize] = currentFPS;
    currentFrame++;

    ImGui::Text("Frame Time: %.4f seconds (%.2f ms)", deltaTime, deltaTime * 1000.0);
    ImGui::Text("FPS: %.2f", currentFPS);
    ImGui::PlotLines("FPS Graph", fpsHistory, historySize, currentFrame % historySize, nullptr, 0.0f, FLT_MAX, ImVec2(0, 80));

    ImGui::End();

    imguiManager->End();
}

void Engine::processInput() {
    GLFWwindow* window = mainWindow->getGLFWwindow();

    static bool escPressedLastFrame = false;
    static bool tabPressedLastFrame = false;
    static bool qPressedLastFrame = false;

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

    int qState = glfwGetKey(window, GLFW_KEY_Q);
    if (qState == GLFW_PRESS && !qPressedLastFrame) {
        toggleWireFrame();
    }
    qPressedLastFrame = (qState == GLFW_PRESS);

    float cameraSpeed = 10.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mainCamera->setPosition(mainCamera->getPosition() + cameraSpeed * mainCamera->getFront());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mainCamera->setPosition(mainCamera->getPosition() - cameraSpeed * mainCamera->getFront());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mainCamera->setPosition(mainCamera->getPosition() - glm::normalize(glm::cross(mainCamera->getFront(), mainCamera->getUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mainCamera->setPosition(mainCamera->getPosition() + glm::normalize(glm::cross(mainCamera->getFront(), mainCamera->getUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        mainCamera->setPosition(mainCamera->getPosition() + glm::vec3(0, cameraSpeed, 0));
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
        mainCamera->setPosition(mainCamera->getPosition() - glm::vec3(0, cameraSpeed, 0));
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

void Engine::toggleWireFrame() {
    wireframeEnabled = !wireframeEnabled;
    if (wireframeEnabled) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

Engine::~Engine() {
	delete mainWindow;
    delete mainCamera;
    delete imguiManager;
    delete shaderProgram;
}