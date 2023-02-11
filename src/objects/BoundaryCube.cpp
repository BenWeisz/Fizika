#include "objects/BoundaryCube.hpp"

BoundaryCube::BoundaryCube() {
    mTransform = new Transform();
    mModel = new Model("../res/models/line-cube.obj", "../res/materials/base.mat");

    Material* mat = mModel->GetMaterial();
    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mat->SetUniformVec3("uFlatColour", glm::vec3(1.0, 1.0, 1.0));
}

BoundaryCube::~BoundaryCube() {
    delete mTransform;
    delete mModel;
}

void BoundaryCube::Draw() const {
    Material* mat = mModel->GetMaterial();

    mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
    mModel->Draw(mTransform);
}

void BoundaryCube::Update() {
}