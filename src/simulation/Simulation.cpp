#include "simulation/Simulation.hpp"

Simulation::~Simulation() {
    // Free the resources for the remaining balls
    for (int i = 0; i < mBalls.size(); i++)
        delete mBalls[i];
}

void Simulation::Update(const double dt) {
    // Add a new ball if the B key is pressed
    if (Input::IsPressed("New Ball")) {
        Eigen::Vector3d q(Rand(), Rand(), Rand());
        q *= 3.0;
        double ztemp = q(2);
        q -= (Eigen::Vector3d::Ones() * 1.5);
        // Ensure that the z component of the position remains positive
        q(2) = ztemp;

        Eigen::Vector3d qdot(Rand(), Rand(), Rand());
        qdot *= 2.5;

        // Add a new ball to the simulation
        Ball* ball = new Ball(q, qdot);
        mBalls.push_back(ball);
    }

    // Keep track of the ball indices to delete
    std::vector<int> deleteIndices;

    const double deletionThreshold = 1.0;
    mPotentialEnergy = 0.0;
    mKineticEnergy = 0.0;
    for (int i = 0; i < mBalls.size(); i++) {
        Ball* ball = mBalls[i];

        ball->Update(dt);
        double potential = ball->GetPotentialEnergy();
        double kinetic = ball->GetKineticEnergy();

        double total = potential + kinetic;
        // If the total energy drops below 1 unit, mark the ball for deletion
        if (total <= deletionThreshold) {
            delete ball;
            deleteIndices.push_back(i);
            continue;
        }

        mPotentialEnergy += potential;
        mKineticEnergy += kinetic;
    }

    // Delete the marked balls accounting for changes in indices in the balls vector
    for (int i = 0; i < deleteIndices.size(); i++)
        mBalls.erase(mBalls.begin() + deleteIndices[i] - i);
}

void Simulation::Draw() const {
    for (int i = 0; i < mBalls.size(); i++)
        mBalls[i]->Draw();
}