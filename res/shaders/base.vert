#version 450 core

uniform mat4 uProjection;
uniform mat4 uCamera;
uniform mat4 uModel;

layout (location = 0) in vec3 iPosition;

void main() {
  vec4 position = uProjection * uCamera * uModel * vec4(iPosition, 1.0);
  gl_Position = position;
}