#include "objects/CardboardBox.hpp"

CardboardBox::CardboardBox() {
    mTexture = new Texture("../res/textures/cardboard.png");
    mTransform = new Transform();
    mModel = new Model("../res/models/cube.obj", "../res/materials/base-texture.mat");

    Material* mat = mModel->GetMaterial();
    mat->SetUniformVec3("uLightColour", Light::GetLightColour());
    mat->SetUniformVec3("uLightPos", Light::GetLightPosition());
    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mat->AddTexture("uTexture0", mTexture);
}

CardboardBox::~CardboardBox() {
    delete mTexture;
    delete mTransform;
    delete mModel;
}

void CardboardBox::Draw() const {
    Material* mat = mModel->GetMaterial();

    glm::mat4 model = mTransform->GetTransformMatrix();

    mat->SetUniformMat4("uModel", model, false);
    mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
    mat->SetUniformVec3("uCameraPos", Camera::GetCameraPos());

    mModel->Draw();
}

void CardboardBox::Update() {
}