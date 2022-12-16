#version 450 core

layout (location = 0) in vec3 iPosition;

int main() {
    gl_Position = vec4(iPosition, 1.0);
}