#include "objects/Plane.hpp"

Plane::Plane() {
    mTransform = new Transform();
    mTexture = new Texture("../res/textures/checker.png");
    mModel = new Model("../res/models/plane-blend.obj", "../res/materials/base-texture.mat");

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
    delete mTransform;
}

void Plane::Draw() const {
    Material* mat = mModel->GetMaterial();
    mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
    mat->SetUniformVec3("uCameraPos", Camera::GetCameraPos());

    mModel->Draw(mTransform);
}

void Plane::Update() {
}
