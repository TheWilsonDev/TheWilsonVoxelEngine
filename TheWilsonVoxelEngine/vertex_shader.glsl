#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 instancePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 pos = aPos + instancePos;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
