#pragma once

#include <Eigen/Core>
#include <glm/glm.hpp>

#include "graphics/Camera.hpp"
#include "graphics/Model.hpp"

class Ball {
   private:
    Eigen::Vector3d qddot;
    double radius;
    const double dampening = 0.95;
    double mass;
    double kineticEnergy;
    double potentialEnergy;
    Model* mModel;

   public:
    Ball(const Eigen::Vector3d& q0, const Eigen::Vector3d& qdot0, const double scale, const double ballMass);
    ~Ball();
    void Draw() const;
    void Update(const double dt);
    double GetPotentialEnergy() const;
    double GetKineticEnergy() const;
    void UpdateEnergies();
    inline double GetDampening() const { return dampening; }
    inline double GetRadius() const { return radius; }
    inline double GetMass() const { return mass; }
    Eigen::Vector3d q;
    Eigen::Vector3d qdot;
};