#version 450 core

uniform vec3 uColour;

flat in vec3 vNormal;

out vec4 oFragColour;

void main() {
  oFragColour = vec4(abs(vNormal), 0.0);
}