#include "objects/Cow.hpp"

Cow::Cow() {
    mTexture = new Texture("../res/textures/spot_texture.png");
    mTransform = new Transform();
    mModel = new Model("../res/models/spot.obj", "../res/materials/base-texture-compute-normals.mat");

    Material* mat = mModel->GetMaterial();
    mat->SetUniformVec3("uLightColour", Light::GetLightColour());
    mat->SetUniformVec3("uLightPos", Light::GetLightPosition());
    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mat->AddTexture("uTexture0", mTexture);

    mTransform->Translate(glm::vec3(0.45, 0.0, 0.0));
}

Cow::~Cow() {
    delete mTexture;
    delete mTransform;
    delete mModel;
}

void Cow::Draw() const {
    Material* mat = mModel->GetMaterial();

    glm::mat4 model = mTransform->GetTransformMatrix();

    mat->SetUniformMat4("uModel", model, false);
    mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
    mat->SetUniformVec3("uCameraPos", Camera::GetCameraPos());

    mModel->Draw();
}

void Cow::Update() {
}