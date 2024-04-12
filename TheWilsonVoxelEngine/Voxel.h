#pragma once
#include <gtc/matrix_transform.hpp>
#include <vector>

class Voxel
{
public:
    glm::vec3 position;

    Voxel();
    Voxel(glm::vec3 pos); 
    void setPosition(const glm::vec3& pos);
    void init();
};

