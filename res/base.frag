#version 450 core

uniform vec3 uColour;

out vec4 oFragColor;

void main() {
    vec3 test = uColour + vec3(1.0, 1.0, 0.0);
    oFragColor = vec4(uColour, 0.0);
}