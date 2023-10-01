#version 450 core

uniform mat4 uLight;
uniform mat4 uModel;

layout (location = 0) in vec3 iPosition;

void main() {
    gl_Position = uLight * uModel * vec4(iPosition, 1.0);
}