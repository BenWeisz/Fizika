#pragma once

#include <Eigen/Core>
#include <glm/glm.hpp>

#include "graphics/Camera.hpp"
#include "graphics/Model.hpp"

class Ball {
   private:
    Eigen::Vector3d q;
    Eigen::Vector3d qdot;
    Eigen::Vector3d qddot;
    const double radius = 0.08;
    const double dampening = 0.99;
    const double simWidth = 3;
    const double simHeight = 3;
    double mass;
    double kineticEnergy;
    double potentialEnergy;
    Model* mModel;

   public:
    Ball(const Eigen::Vector3d& q0, const Eigen::Vector3d& qdot0);
    ~Ball();
    void Draw() const;
    void Update(const double dt);
    double GetPotentialEnergy() const;
    double GetKineticEnergy() const;
};