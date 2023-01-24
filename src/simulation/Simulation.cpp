
#include "simulation/Simulation.hpp"

Simulation::Simulation(const double simWidth, const double simHeight) : mCollisionIsOn(true), mSimWidth(simWidth), mSimHeight(simHeight) {}

Simulation::~Simulation() {
    // Free the resources for the remaining balls
    for (int i = 0; i < mBalls.size(); i++)
        delete mBalls[i];
}

void Simulation::Update(const double dt) {
    // Toggle the collision ball-ball collision checks
    if (Input::IsPressed("Toggle Collisions"))
        mCollisionIsOn = !mCollisionIsOn;

    // Add a new ball if the B key is pressed
    if (Input::IsPressed("New Ball"))
        AddNewBall();

    for (int i = 0; i < mBalls.size(); i++) {
        Ball* ball1 = mBalls[i];
        ball1->Update(dt);

        if (mCollisionIsOn) {
            for (int j = i + 1; j < mBalls.size(); j++) {
                Ball* ball2 = mBalls[j];
                HandleBallCollision(ball1, ball2, 1.0);
            }
        }

        HandleWallCollision(ball1);
    }

    // Update Energies and Delete Dead Balls
    DeletionPass();
}

void Simulation::Draw() const {
    for (int i = 0; i < mBalls.size(); i++)
        mBalls[i]->Draw();
}

void Simulation::HandleWallCollision(Ball* ball) {
    double w = mSimWidth / 2.0;
    double h = mSimHeight / 2.0;
    double radius = ball->GetRadius();
    double dampening = ball->GetDampening();

    if (ball->q(0) < -w + radius) {
        ball->q(0) = -w + radius;
        ball->qdot(0) = -dampening * ball->qdot(0);
    } else if (ball->q(0) > w - radius) {
        ball->q(0) = w - radius;
        ball->qdot(0) = -dampening * ball->qdot(0);
    }
    if (ball->q(1) < -h + radius) {
        ball->q(1) = -h + radius;
        ball->qdot(1) = -dampening * ball->qdot(1);
    } else if (ball->q(1) > h - radius) {
        ball->q(1) = h - radius;
        ball->qdot(1) = -dampening * ball->qdot(1);
    }
    if (ball->q(2) < radius) {
        ball->q(2) = radius;
        ball->qdot(2) = -dampening * ball->qdot(2);
    } else if (ball->q(2) > 3 - radius) {
        ball->q(2) = 3 - radius;
        ball->qdot(2) = -dampening * ball->qdot(2);
    }
}

void Simulation::HandleBallCollision(Ball* ball1, Ball* ball2, const double coeffRestitution) {
    Eigen::Vector3d collisionDir = ball2->q - ball1->q;
    const double distance = collisionDir.norm();

    const double r1 = ball1->GetRadius();
    const double r2 = ball2->GetRadius();

    if (distance == 0.0 || distance > ball1->GetRadius() + ball2->GetRadius())
        return;

    collisionDir.normalize();

    // Divide the residual into 2 parts, half will be added to ball 1 and half to ball 2
    const double correction = (r1 + r2 - distance) / 2.0;

    ball1->q += collisionDir * -correction;
    ball2->q += collisionDir * correction;

    const double v1 = ball1->qdot.dot(collisionDir);
    const double v2 = ball2->qdot.dot(collisionDir);

    const double m1 = ball1->GetMass();
    const double m2 = ball2->GetMass();

    const double newV1 = ((m1 * v1) + (m2 * v2) - (m2 * coeffRestitution * (v1 - v2))) / (m1 + m2);
    const double newV2 = ((m1 * v1) + (m2 * v2) - (m1 * coeffRestitution * (v2 - v1))) / (m1 + m2);

    ball1->qdot += collisionDir * (newV1 - v1);
    ball2->qdot += collisionDir * (newV2 - v2);
}

void Simulation::AddNewBall() {
    Eigen::Vector3d q(Rand(), Rand(), Rand());
    q *= 3.0;
    double ztemp = q(2);
    q -= (Eigen::Vector3d::Ones() * 1.5);
    // Ensure that the z component of the position remains positive
    q(2) = ztemp;

    Eigen::Vector3d qdot(Rand(), Rand(), Rand());
    qdot *= 2.5;

    double rs = (Rand() * 2.5) + 1.0;
    double scale = rs;
    double mass = (4.0 / 3.0) * 3.141592 * (rs * rs * rs);

    // Add a new ball to the simulation
    Ball* ball = new Ball(q, qdot, scale, mass);
    mBalls.push_back(ball);
}

void Simulation::DeletionPass() {
    std::vector<int> deleteIndices;

    const double deletionThreshold = 0.2;
    mPotentialEnergy = 0.0;
    mKineticEnergy = 0.0;
    for (int i = 0; i < mBalls.size(); i++) {
        Ball* ball = mBalls[i];

        ball->UpdateEnergies();
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