#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch, float fov, float aspectRatio, float nearPlane, float farPlane);

    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3& position);

    glm::vec3 getFront() const;
    glm::vec3 getUp() const;
    glm::vec3 getRight() const;

    float getYaw() const;
    void setYaw(float yaw);

    float getPitch() const;
    void setPitch(float pitch);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

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

    void updateCameraVectors();
};
