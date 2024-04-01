#include "Camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch, float fov, float aspectRatio, float nearPlane, float farPlane)
    : position(position), worldUp(up), yaw(yaw), pitch(pitch), fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane) {
    updateCameraVectors();
}

glm::vec3 Camera::getPosition() const {
    return position;
}

void Camera::setPosition(const glm::vec3& position) {
    this->position = position;
}

glm::vec3 Camera::getFront() const {
    return front;
}

glm::vec3 Camera::getUp() const {
    return up;
}

glm::vec3 Camera::getRight() const {
    return right;
}

float Camera::getYaw() const {
    return yaw;
}

void Camera::setYaw(float yaw) {
    this->yaw = yaw;
    updateCameraVectors();
}

float Camera::getPitch() const {
    return pitch;
}

void Camera::setPitch(float pitch) {
    this->pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
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
