#version 450 core

uniform mat4 uProjection;
uniform mat4 uCamera;
uniform mat4 uCorrection;

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;

flat out vec3 vNormal;

void main() {
  vec4 position = uProjection * uCamera * uCorrection * vec4(iPosition, 1.0);

  vNormal = iNormal;

  gl_Position = position;
}