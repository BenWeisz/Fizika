#version 450 core

layout(location = 0) in vec3 iPosition;

void main() {
  mat4 correction = mat4(0.0, 0.0, 1.0, 0.0,
                         1.0, 0.0, 0.0, 0.0,
                         0.0, 1.0, 0.0, 0.0,
                         0.0, 0.0, 0.0, 1.0);

  vec3 position = vec3(correction * vec4(iPosition, 1.0));

  gl_Position = vec4(position, 1.0);
}