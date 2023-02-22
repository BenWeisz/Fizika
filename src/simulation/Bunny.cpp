#include "simulation/Bunny.hpp"

Bunny::Bunny() {
    mTransform = new Transform();
    mModel = new Model("../res/models/coarse_bunny.obj", "../res/materials/base-flat.mat");

    Transform* baseTransform = mModel->GetBaseTransform();
    baseTransform->Scale(glm::vec3(0.01, 0.01, 0.01));
    baseTransform->Rotate(glm::vec3(1.0, 0.0, 0.0), 90.0f);
    baseTransform->Rotate(glm::vec3(0.0, 0.0, 1.0), 90.0f);
    baseTransform->Translate(glm::vec3(0.0, 0.0, -0.25));

    Material* mat = mModel->GetMaterial();
    mat->SetUniformVec3("uLightColour", Light::GetLightColour());
    mat->SetUniformVec3("uLightPos", Light::GetLightPosition());
    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mat->SetUniformVec3("uFlatColour", glm::vec3(1.0f, 0.0f, 0.0f));
}

Bunny::~Bunny() {
    delete mModel;
    delete mTransform;
}

void Bunny::Draw() const {
    Material* mat = mModel->GetMaterial();
    mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
    mat->SetUniformVec3("uCameraPos", Camera::GetCameraPos());

    mModel->Draw(mTransform);
}

void Bunny::Update() {
}