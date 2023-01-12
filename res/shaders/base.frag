#version 450 core

uniform vec3 uColour;

out vec4 oFragColour;

void main() {
  oFragColour = vec4(uColour, 0.0);
}