#version 450 core

uniform vec3 uColour;

out vec4 oFragColor;

void main() {
  oFragColor = vec4(uColour, 0.0);
}