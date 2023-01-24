#include "simulation/Ball.hpp"

Ball::Ball(const Eigen::Vector3d& q0, const Eigen::Vector3d& qdot0, const double scale, const double ballMass) : q(q0), qdot(qdot0), mass(ballMass) {
    qddot = Eigen::Vector3d(0.0, 0.0, -10.0);

    mModel = new Model("../res/models/ball.obj", "../res/materials/base.mat");
    Material* mat = mModel->GetMaterial();
    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mat->SetUniformVec3("uFlatColour", glm::vec3(0.0, 1.0, 0.0));

    radius = 0.08 * scale;

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

    double ballScale = radius / 0.08;
    uModel = glm::scale(uModel, glm::vec3(ballScale));
    mat->SetUniformMat4("uModel", uModel, false);
    mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
    mModel->Draw();
}

void Ball::Update(const double dt) {
    qdot = qdot + (dt * qddot);
    q = q + (dt * qdot);
}

void Ball::UpdateEnergies() {
    potentialEnergy = (-qddot).dot(q - Eigen::Vector3d(0.0, 0.0, radius)) * mass;
    kineticEnergy = 0.5 * mass * qdot.dot(qdot);
}

double Ball::GetPotentialEnergy() const {
    return potentialEnergy;
}

double Ball::GetKineticEnergy() const {
    return kineticEnergy;
}