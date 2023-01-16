#include "simulation/Ball.hpp"

Ball::Ball(const Eigen::Vector3d& q0, const Eigen::Vector3d& qdot0) : q(q0), qdot(qdot0) {
    qddot = Eigen::Vector3d(0.0, 0.0, -10.0);

    mModel = new Model("../res/models/ball.obj", "../res/base.mat");
    Material* mat = mModel->GetMaterial();
    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mat->SetUniformVec3("uFlatColour", glm::vec3(0.0, 1.0, 0.0));

    mass = 1.0;

    potentialEnergy = (-qddot).dot(q - Eigen::Vector3d(0.0, 0.0, radius)) * mass;
    kineticEnergy = 0.5 * mass * qdot.dot(qdot);
}

Ball::~Ball() {
    delete mModel;
}

void Ball::Draw() const {
    Material* mat = mModel->GetMaterial();

    // Construct Model Matrix for Ball
    glm::mat4 uModel = glm::mat4(1.0);
    uModel = glm::translate(uModel, glm::vec3(q(0), q(1), q(2)));

    // Correction for bad model
    uModel = glm::translate(uModel, glm::vec3(0.0, -0.015, 0.0));
    uModel = glm::scale(uModel, glm::vec3(0.03, 0.03, 0.03));
    mat->SetUniformMat4("uModel", uModel, false);
    mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
    mModel->Draw();
}

void Ball::Update(const double dt) {
    int n = 5;
    double sdt = dt / n;
    double w = simWidth / 2.0;
    double h = simHeight / 2.0;

    for (int i = 0; i < n; i++) {
        qdot = qdot + (sdt * qddot);
        q = q + (sdt * qdot);

        if (q(0) < -w + radius) {
            q(0) = -w + radius;
            qdot(0) = -dampening * qdot(0);
        } else if (q(0) > w - radius) {
            q(0) = w - radius;
            qdot(0) = -dampening * qdot(0);
        }
        if (q(1) < -h + radius) {
            q(1) = -h + radius;
            qdot(1) = -dampening * qdot(1);
        } else if (q(1) > h - radius) {
            q(1) = h - radius;
            qdot(1) = -dampening * qdot(1);
        }
        if (q(2) < radius) {
            q(2) = radius;
            qdot(2) = -dampening * qdot(2);
        }
    }

    potentialEnergy = (-qddot).dot(q - Eigen::Vector3d(0.0, 0.0, radius)) * mass;
    kineticEnergy = 0.5 * mass * qdot.dot(qdot);
}

double Ball::GetPotentialEnergy() const {
    return potentialEnergy;
}

double Ball::GetKineticEnergy() const {
    return kineticEnergy;
}