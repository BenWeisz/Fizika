#include "graphics/Transform.hpp"

Transform::Transform(const glm::vec3& scale, const glm::vec3& rotationAxis, const double rotationTheta, const glm::vec3& translation) : mScale(scale), mTranslation(translation) {
    mRotations.push_back({rotationAxis, rotationTheta});
}

Transform::Transform() : mScale(glm::vec3(1.0, 1.0, 1.0)), mTranslation(glm::vec3(0.0, 0.0, 0.0)) {
    mRotations.push_back({glm::vec3(1.0, 0.0, 0.0), 0.0});
}

glm::mat4 Transform::GetTransformMatrix() const {
    glm::mat4 transform = glm::mat4(1.0);
    transform = glm::translate(transform, mTranslation);

    // Apply the rotations in reverse order to preserve user specified order
    for (int i = mRotations.size() - 1; i >= 0; i--) {
        auto rotation = mRotations[i];
        transform = glm::rotate(transform, glm::radians((float)rotation.second), rotation.first);
    }

    transform = glm::scale(transform, mScale);

    return transform;
}
