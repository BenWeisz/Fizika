#include "objects/Box.hpp"

Box::Box() {
    mTransform = new Transform();
    mModel = new Model("../res/meshes/cube.obj", "../res/materials/base-flat.mat");

    Material* mat = mModel->GetMaterial();
    DirectionalLight* light = LightManager::GetDirectionalLight(0);
    if (light != nullptr) {
        mat->SetUniformVec3("uLightColour", light->color);
        mat->SetUniformVec3("uLightDir", light->direction);
    }

    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mat->SetUniformVec3("uFlatColour", glm::vec3(0.0f, 0.0f, 1.0f));
}

Box::~Box() {
    delete mModel;
    delete mTransform;
}

void Box::Draw(const Window::RenderPassType& renderPassType) const {
    if (renderPassType == Window::RenderPassType::OPAQUE) {
        Material* mat = mModel->GetMaterial();
        mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
        mat->SetUniformVec3("uCameraPos", Camera::GetCameraPos());
    }

    mModel->Draw(mTransform, renderPassType);
}

void Box::Update() {
}