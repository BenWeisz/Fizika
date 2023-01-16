#include "simulation/Cube.hpp"

Cube::Cube(const glm::vec3& position) : mPosition(position) {
    mModel = new Model("../res/models/line-cube.obj", "../res/base.mat");
    Material* mat = mModel->GetMaterial();
    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mat->SetUniformVec3("uFlatColour", glm::vec3(1.0, 1.0, 1.0));
}

Cube::~Cube() {
    delete mModel;
}

void Cube::Draw() const {
    Material* mat = mModel->GetMaterial();

    glm::mat4 uModel = glm::mat4(1.0);
    uModel = glm::translate(uModel, mPosition);
    uModel = glm::scale(uModel, glm::vec3(3.0, 3.0, 3.0));
    mat->SetUniformMat4("uModel", uModel, false);

    mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
    mModel->Draw();
}