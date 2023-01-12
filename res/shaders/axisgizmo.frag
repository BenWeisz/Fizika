#version 450 core

flat in vec3 vFragColour;

out vec4 oFragColour;

void main() {
    oFragColour = vec4(vFragColour, 0.0);
}