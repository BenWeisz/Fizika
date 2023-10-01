#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
struct PointLight {
    glm::vec3 color;
    glm::vec3 position;
};

struct DirectionalLight {
    glm::vec3 color;
    glm::vec3 direction;
    glm::vec3 position;  // For shadow mapping

    glm::mat4 GetLightSpaceMatrix() {
        float nearPlane = 1.0f;
        float farPlane = 7.5f;
        // These coordinates aren't flipped by the camera
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
        glm::mat4 lightView = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        return lightSpaceMatrix;
    }
};