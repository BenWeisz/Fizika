#version 450 core

uniform mat4 uProjection;
uniform mat4 uModel;

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iFragColour;

flat out vec3 vFragColour;

void main() {
    gl_Position = uProjection * uModel * vec4(iPosition, 1.0);
    vFragColour = iFragColour;
}