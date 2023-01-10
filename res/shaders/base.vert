#version 450 core

uniform mat4 uProj;
uniform mat4 uCam;
// uniform mat4 uCor;

layout(location = 0) in vec3 iPosition;

void main() {
  vec4 position = uProj * uCam * vec4(iPosition, 1.0);

  gl_Position = position;
}