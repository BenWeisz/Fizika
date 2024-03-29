#version 450 core

uniform vec3 uFlatColour;

uniform vec3 uLightColour;
uniform vec3 uLightDir;
uniform vec3 uCameraPos;

flat in vec3 vNormal;
in vec3 vFragPos;

out vec4 oFragColour;

void main() {
  float ambient = 0.2;

  vec3 normal = normalize(vNormal);

  vec3 lightDir = normalize(-uLightDir);
  float diffuse = max(0.0, dot(normal, lightDir));

  float specular = 0.0;

  if (diffuse > 0.0) {
    vec3 viewDir = normalize(uCameraPos - vFragPos);
    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    float specular = pow(max(0.0, dot(viewDir, reflectDir)), 32.0);
  }

  vec3 colour = (ambient + diffuse + specular) * uFlatColour * uLightColour;
  
  oFragColour = vec4(colour, 0.0);
}