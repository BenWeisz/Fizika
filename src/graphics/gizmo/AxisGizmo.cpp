#include "graphics/gizmo/AxisGizmo.hpp"

AxisGizmo::AxisGizmo(const double screenX, const double screenY) : mScreenX(screenX), mScreenY(screenY) {
    // Define the vertex data for the axis
    std::vector<GLfloat> data = {
        0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 1.f, 0.f, 0.f, 1.f};

    mVBO = new VertexBuffer(data);

    // Define the index buffer data for the axis
    std::vector<GLuint> indexData = {
        0, 1, 0, 2, 0, 3};

    mIBO = new IndexBuffer(indexData);

    // Define the vertex buffer layout for the data
    VertexBufferLayout layout;
    layout.AddLayoutElement("iPosition", 3);
    layout.AddLayoutElement("iFragColour", 3);

    // Define the shader for the axis gizmo
    mShader = new Shader("../res/shaders/axisgizmo.vert", "../res/shaders/axisgizmo.frag");

    // Define the vertex array for the gizmo
    mVAO = new VertexArray();

    // Bundle all of the data and the shader
    mVAO->Bundle(mVBO, mIBO, &layout, mShader);
}

AxisGizmo::~AxisGizmo() {
    delete mVBO;
    delete mIBO;
    delete mVAO;
    delete mShader;
}

void AxisGizmo::Draw() const {
    mVAO->Bind();
    mShader->Bind();

    glm::mat4 model = glm::scale(glm::mat4(1.0), glm::vec3(0.25));

    // Set the basic transforms for the gizmo
    mShader->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
    mShader->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);  // This is the one to modify to be in the top right
    mShader->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mShader->SetUniformMat4("uModel", model, false);

    glDrawElements(GL_LINES, mIBO->GetCount(), GL_UNSIGNED_INT, (void*)0);

    mShader->Unbind();
    mVAO->Unbind();
}
