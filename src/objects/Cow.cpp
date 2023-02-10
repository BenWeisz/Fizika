#include "objects/Cow.hpp"

Cow::Cow(const double initialPosition, const double initialVelocity, const double mass, const double springK, const double restLength) : mPosition(initialPosition), mVelocity(initialVelocity), mMass(mass), mSpringK(springK), mRestLength(restLength) {
    mTexture = new Texture("../res/textures/spot_texture.png");
    mTransform = new Transform();
    mModel = new Model("../res/models/spot.obj", "../res/materials/base-texture-compute-normals.mat");

    Material* mat = mModel->GetMaterial();
    mat->SetUniformVec3("uLightColour", Light::GetLightColour());
    mat->SetUniformVec3("uLightPos", Light::GetLightPosition());
    mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mat->AddTexture("uTexture0", mTexture);

    mTransform->Rotate(glm::vec3(1.0, 0.0, 0.0), 90.0);
    mTransform->Rotate(glm::vec3(0.0, 0.0, 1.0), -90.0);
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

void Cow::Update(const double dt) {
    // mVelocity = (((mMass * mVelocity) + (dt * (mSpringK) * (-mPosition + mRestLength))) / (mMass - (dt * mSpringK)));
    // mPosition += dt * mVelocity;

    Eigen::Vector2d yp = IntegrateBackward(dt);
    mPosition = yp(0);
    mVelocity = yp(1);

    mTransform->SetTranslation(glm::vec3(0.45, 0.0, mPosition));
}

Eigen::Vector2d Cow::IntegrateBackward(const double dt) const {
    Eigen::Vector3d y;
    y << 1, mPosition, mVelocity;

    Eigen::Matrix3d A;
    A << 1.0, 0.0, 0.0,
        0.0, 1.0, -dt,
        (-mRestLength * mSpringK * dt) / mMass, (dt * mSpringK) / mMass, 1.0;

    Eigen::Matrix3d Ainv = Eigen::Inverse(A);
    Eigen::Vector3d yp = Ainv * y;

    return Eigen::Vector2d({yp(1), yp(2)});
}

double Cow::ComputeKineticEnergy() const {
    return 0.5 * mMass * mVelocity * mVelocity;
}

double Cow::ComputePotentialEnergy() const {
    double residual = mPosition - mRestLength;
    return 0.5 * mSpringK * residual * residual;
}