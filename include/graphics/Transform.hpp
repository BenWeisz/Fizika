#pragma once

#include <vector>
#include <utility>

#include <glm/gtc/matrix_transform.hpp>

class Transform {
   private:
    glm::vec3 mScale;
    std::vector<std::pair<glm::vec3, double>> mRotations;
    glm::vec3 mTranslation;

   public:
    Transform(const glm::vec3& scale, const glm::vec3& rotationAxis, const double rotationTheta, const glm::vec3& translation);
    glm::mat4 GetTransformMatrix() const;
    inline void SetScale(const glm::vec3& scale) { mScale = scale; }
    inline void SetRotation(const glm::vec3& rotationAxis, const double rotationTheta) {
        mRotations.clear();
        mRotations.push_back({rotationAxis, rotationTheta});
    }
    inline void SetTranslation(const glm::vec3& translation) { mTranslation = translation; }
    inline void Scale(const glm::vec3& scale) { mScale *= scale; }
    inline void Rotate(const glm::vec3& rotationAxis, const double rotationTheta) { mRotations.push_back({rotationAxis, rotationTheta}); }
    inline void Translate(const glm::vec3& translation) { mTranslation += translation; }
};