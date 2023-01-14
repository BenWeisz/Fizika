#version 450 core

uniform mat4 uProjection;
uniform mat4 uCamera;
uniform mat4 uCorrection;
uniform mat4 uModel;

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;

flat out vec3 vNormal;
out vec3 vFragPos;

void main() {
  vec4 position = uProjection * uCamera * uCorrection * uModel * vec4(iPosition, 1.0);


  gl_Position = position;
  vNormal = vec3(transpose(inverse(uModel)) * vec4(iNormal, 1.0));
  vFragPos = iPosition;
}