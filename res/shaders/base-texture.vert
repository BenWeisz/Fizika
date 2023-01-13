#version 450 core

uniform mat4 uProjection;
uniform mat4 uCamera;
uniform mat4 uCorrection;

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTextureUV;

flat out vec3 vNormal;
out vec3 vFragPos;
out vec2 vTextureUV;

void main() {
  vec4 position = uProjection * uCamera * uCorrection * vec4(iPosition, 1.0);

  vNormal = iNormal;

  gl_Position = position;
  vFragPos = iPosition;
  vTextureUV = iTextureUV;
}