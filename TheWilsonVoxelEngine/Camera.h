#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch, float fov, float aspectRatio, float nearPlane, float farPlane, float sensitivity);

    glm::vec3 getPosition() const { return position; }
    void setPosition(const glm::vec3& position) { this->position = position; }

    float getNearPlane() const { return nearPlane; }
    float getFarPlane() const { return farPlane; }

    glm::vec3 getFront() const { return front; }
    glm::vec3 getUp() const { return up; }
    glm::vec3 getRight() const { return right; }

    float getYaw() const { return yaw; }
    void setYaw(float yaw);

    float getPitch() const { return pitch; }
    void setPitch(float pitch);

    float getFOV() const { return fov; }
    void setFOV(float newFov);

    glm::mat4 getViewMatrix() const { return glm::lookAt(position, position + front, up); }
    glm::mat4 getProjectionMatrix() const { return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane); }

    float getSensitivity() const { return sensitivity; }
    void setSensitivity(float sensitivity) { this->sensitivity = sensitivity;  }

    void processCamMouseMovement(float xoffset, float yoffset, bool constrainPitch);

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    float sensitivity;

    void updateCameraVectors();
};
