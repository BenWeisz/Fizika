#version 450 core

layout(location = 0) in vec2 iPosition;

void main() { gl_Position = vec4(iPosition, 1.0, 1.0); }