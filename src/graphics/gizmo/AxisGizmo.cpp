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

    float yaw = Camera::GetYaw();
    float pitch = Camera::GetPitch();

    glm::mat4 model = glm::mat4(1.0);

    auto screenDimensions = Window::GetDimensions();
    double screenWidth = (double)screenDimensions.first;
    double screenHeight = (double)screenDimensions.second;

    double aspectRatio = screenWidth / screenHeight;
    double yOffset = (mScreenX / screenWidth) * (2.0 * aspectRatio) - aspectRatio;
    double zOffset = (mScreenY / screenHeight) * 2.0 - 1.0;

    // Transformations are applied in reverse order of execution
    model = glm::translate(model, glm::vec3(0.0, yOffset, zOffset));
    model = glm::rotate(model, glm::radians(-yaw), glm::vec3(0.0, 0.0, 1.0));
    model = glm::rotate(model, glm::radians(pitch), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(0.1));

    glm::mat4 ortho = glm::ortho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);

    // Set the basic transforms for the gizmo
    mShader->SetUniformMat4("uProjection", ortho, false);
    mShader->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
    mShader->SetUniformMat4("uModel", model, false);

    glDrawElements(GL_LINES, mIBO->GetCount(), GL_UNSIGNED_INT, (void*)0);

    mShader->Unbind();
    mVAO->Unbind();
}