#include "graphics/gizmo/AxisGizmo.hpp"

AxisGizmo::AxisGizmo(const double screenX, const double screenY) : mScreenX(screenX), mScreenY(screenY) {
    // Define the vertex data for the axis
    std::vector<GLfloat> data = {
        0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
        .9f, .1f, -.1f, 0.f, 0.f, 0.f,
        .9f, -.1f, .1f, 0.f, 0.f, 0.f,
        .1f, .9f, -.1f, 0.f, 0.f, 0.f,
        -.1f, .9f, .1f, 0.f, 0.f, 0.f,
        .1f, -.1f, .9f, 0.f, 0.f, 0.f,
        -.1f, .1f, .9f, 0.f, 0.f, 0.f};

    mVBO = new VertexBuffer(data);

    // Define the index buffer data for the axis
    std::vector<GLuint> indexData = {
        0, 1, 0, 2, 0, 3, 4, 1, 5, 1, 6, 2, 7, 2, 8, 3, 9, 3};

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
    float screenWidth = (float)screenDimensions.first;
    float screenHeight = (float)screenDimensions.second;

    float aspectRatio = screenWidth / screenHeight;
    float yOffset = (mScreenX / screenWidth) * (2.0f * aspectRatio) - aspectRatio;
    float zOffset = (mScreenY / screenHeight) * 2.0f - 1.0f;

    glm::vec3 pitchAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    pitchAxis = glm::rotate(pitchAxis, glm::radians(yaw), glm::vec3(0.0f, 0.0f, 1.0f));

    // Transformations are applied in reverse order of execution
    model = glm::translate(model, glm::vec3(0.0f, yOffset, zOffset));
    model = glm::rotate(model, glm::radians(-yaw), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(pitch), pitchAxis);
    model = glm::scale(model, glm::vec3(0.1f));

    glm::mat4 correction = glm::mat4(1.0f);
    correction = glm::rotate(correction, -glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
    correction = glm::rotate(correction, -glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 ortho = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
    ortho = ortho * correction;

    // Set the basic transforms for the gizmo
    mShader->SetUniformMat4("uProjection", ortho, false);
    mShader->SetUniformMat4("uModel", model, false);

    glDrawElements(GL_LINES, mIBO->GetCount(), GL_UNSIGNED_INT, (void*)0);

    mShader->Unbind();
    mVAO->Unbind();
}
