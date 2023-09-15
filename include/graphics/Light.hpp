#pragma once

#include <glm/glm.hpp>

struct PointLight {
    glm::vec3 color;
    glm::vec3 position;
};

struct DirectionalLight {
    glm::vec3 color;
    glm::vec3 direction;
    glm::vec3 position;  // For shadow mapping
};