#include "objects/Plane.hpp"

Plane::Plane() {
    mTransform = new Transform();
    mTexture = new Texture("../res/textures/checker.png");
    mModel = new Model("../res/meshes/plane.obj", "../res/materials/base-texture.mat");

    Material* mat = mModel->GetMaterial();

    DirectionalLight* light = LightManager::GetDirectionalLight(0);
    if (light != nullptr) {
        mat->SetUniformVec3("uLightColour", light->color);
        mat->SetUniformVec3("uLightPos", light->direction);
    }

    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
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
