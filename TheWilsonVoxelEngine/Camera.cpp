#include "Camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch, float fov, float aspectRatio, float nearPlane, float farPlane, float sensitivity)
    : position(position), worldUp(up), yaw(yaw), pitch(pitch), fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane), sensitivity(sensitivity){
    updateCameraVectors();
}


void Camera::setYaw(float yaw) {
    this->yaw = yaw;
    updateCameraVectors();
}

void Camera::setPitch(float pitch) {
    this->pitch = pitch;
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::processCamMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Prevent flipping when looking to far down
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}