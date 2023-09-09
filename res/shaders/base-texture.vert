#version 450 core

uniform mat4 uProjection;
uniform mat4 uCamera;
uniform mat4 uModel;

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTextureUV;

flat out vec3 vNormal;
out vec3 vFragPos;
out vec2 vTextureUV;

void main() {
  vec4 position = uProjection * uCamera * uModel * vec4(iPosition, 1.0);

  gl_Position = position;
  vFragPos = vec3(uModel * vec4(iPosition, 1.0));
  vNormal = vec3(transpose(inverse(uModel)) * vec4(iNormal, 1.0));
  vTextureUV = iTextureUV;
}