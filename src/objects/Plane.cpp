#include "objects/Plane.hpp"

Plane::Plane() : mScale(glm::vec3(1.0, 1.0, 1.0)),
                 mRotationAxis(glm::vec3(1.0, 0.0, 0.0)),
                 mRotationTheta(0.0),
                 mTranslation(glm::vec3(0.0, 0.0, 0.0)) {
    mTexture = new Texture("../res/textures/checker.png");
    mModel = new Model(Geometry::PLANE, "../res/materials/base-texture.mat");

    Material* mat = mModel->GetMaterial();
    mat->SetUniformVec3("uLightColour", Light::GetLightColour());
    mat->SetUniformVec3("uLightPos", Light::GetLightPosition());
    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mat->AddTexture("uTexture0", mTexture);
}

Plane::~Plane() {
    delete mTexture;
    delete mModel;
}

void Plane::Draw() const {
    Material* mat = mModel->GetMaterial();

    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, mTranslation);
    model = glm::rotate(model, glm::radians((float)mRotationTheta), mRotationAxis);
    model = glm::scale(model, mScale);

    mat->SetUniformMat4("uModel", model, false);
    mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
    mat->SetUniformVec3("uCameraPos", Camera::GetCameraPos());

    mModel->Draw();
}

void Plane::Update() {
}

void Plane::SetScale(const glm::vec3& scale) {
    mScale = scale;
}

void Plane::SetRotation(const glm::vec3& axis, const double theta) {
    mRotationAxis = axis;
    mRotationTheta = theta;
}

void Plane::SetTranslation(const glm::vec3& translation) {
    mTranslation = translation;
}