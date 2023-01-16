#pragma once

#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>

#include <Eigen/Core>

#include "input/Input.hpp"
#include "Ball.hpp"

class Simulation {
   private:
    std::vector<Ball*> mBalls;
    double mPotentialEnergy;
    double mKineticEnergy;

   public:
    Simulation() = default;
    ~Simulation();
    void Update(const double dt);
    void Draw() const;
    inline double GetPotentialEnergy() const { return mPotentialEnergy; }
    inline double GetKineticEnergy() const { return mKineticEnergy; }
    inline int GetNumberOfBalls() const { return mBalls.size(); }

   private:
    static double Rand() {
        const int resolution = 8;
        double v = (double)rand();
        double temp = v / std::pow(10, resolution);
        v = temp - (int)temp;
        return v;
    }
};