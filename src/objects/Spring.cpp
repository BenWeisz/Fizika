#include "objects/Spring.hpp"

Spring::Spring() {
    mTransform = new Transform();
    mModel = new Model("../res/models/spring.obj", "../res/materials/base-smooth.mat");

    Material* mat = mModel->GetMaterial();
    mat->SetUniformVec3("uLightColour", Light::GetLightColour());
    mat->SetUniformVec3("uLightPos", Light::GetLightPosition());
    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mat->SetUniformVec3("uFlatColour", glm::vec3(1.0, 0.0, 0.0));

    mPreviousScale = 1.0;
}

Spring::~Spring() {
    delete mTransform;
    delete mModel;
}

void Spring::Draw() const {
    Material* mat = mModel->GetMaterial();

    glm::mat4 model = mTransform->GetTransformMatrix();

    mat->SetUniformMat4("uModel", model, false);
    mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
    mat->SetUniformVec3("uCameraPos", Camera::GetCameraPos());

    mModel->Draw();
}

void Spring::Update(const double cowPosition) {
    double scale = (cowPosition - 0.175) / (2.0 - 0.175);

    // Unscale last scale
    mTransform->Scale(glm::vec3(1.0, 1 / mPreviousScale, 1.0));

    // Scale by new factor
    mTransform->Scale(glm::vec3(1.0, scale, 1.0));

    // Set the previous scale
    mPreviousScale = scale;
}