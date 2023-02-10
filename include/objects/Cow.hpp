#pragma once

#include <glm/glm.hpp>
#include <Eigen/Core>
#include <Eigen/LU>

#include "graphics/Camera.hpp"
#include "graphics/Model.hpp"
#include "graphics/Material.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Light.hpp"
#include "graphics/Transform.hpp"

class Cow {
   private:
    Texture* mTexture;
    Transform* mTransform;
    Model* mModel;
    double mPosition;
    double mVelocity;
    double mMass;
    double mSpringK;
    double mRestLength;

   public:
    Cow(const double initialPosition, const double initialVelocity, const double mass, const double springK, const double restLength);
    ~Cow();
    inline Transform* GetTransform() const { return mTransform; }
    void Draw() const;
    void Update(const double dt);
    Eigen::Vector2d IntegrateBackward(const double dt) const;
    double ComputeKineticEnergy() const;
    double ComputePotentialEnergy() const;
    inline double GetPosition() const { return mPosition; }
};