#version 450 core

uniform vec3 uFlatColour;

out vec4 oFragColour;

void main() {
  oFragColour = vec4(uFlatColour, 0.0);
}