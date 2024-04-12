#include "Voxel.h"

Voxel::Voxel() : position(0.0f, 0.0f, 0.0f) {
    init();
}

Voxel::Voxel(glm::vec3 pos) : position(pos) {
    init(); 
}

void Voxel::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Voxel::init() {

}