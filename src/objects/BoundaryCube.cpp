#include "objects/BoundaryCube.hpp"

BoundaryCube::BoundaryCube() : mScale(glm::vec3(1.0, 1.0, 1.0)),
                               mRotationAxis(glm::vec3(1.0, 0.0, 0.0)),
                               mRotationTheta(0.0),
                               mTranslation(glm::vec3(0.0, 0.0, 0.0)) {
    mModel = new Model("../res/models/line-cube.obj", "../res/materials/base.mat");
    Material* mat = mModel->GetMaterial();
    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mat->SetUniformVec3("uFlatColour", glm::vec3(1.0, 1.0, 1.0));
}

BoundaryCube::~BoundaryCube() {
    delete mModel;
}

void BoundaryCube::Draw() const {
    Material* mat = mModel->GetMaterial();

    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, mTranslation);
    model = glm::rotate(model, glm::radians((float)mRotationTheta), mRotationAxis);
    model = glm::scale(model, mScale);
    mat->SetUniformMat4("uModel", model, false);

    mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
    mModel->Draw();
}

void BoundaryCube::Update() {
}

void BoundaryCube::SetScale(const glm::vec3& scale) {
    mScale = scale;
}

void BoundaryCube::SetRotation(const glm::vec3& axis, const double theta) {
    mRotationAxis = axis;
    mRotationTheta = theta;
}

void BoundaryCube::SetTranslation(const glm::vec3& translation) {
    mTranslation = translation;
}