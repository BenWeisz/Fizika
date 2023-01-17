#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/Camera.hpp"
#include "graphics/Model.hpp"
#include "graphics/Material.hpp"

class BoundaryCube {
   private:
    glm::vec3 mScale;
    glm::vec3 mRotationAxis;
    double mRotationTheta;
    glm::vec3 mTranslation;
    Model* mModel;

   public:
    BoundaryCube();
    ~BoundaryCube();
    void Draw() const;
    void Update();
    void SetScale(const glm::vec3& scale);
    void SetRotation(const glm::vec3& rotation, const double theta);
    void SetTranslation(const glm::vec3& translation);
};