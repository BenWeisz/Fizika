#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "graphics/Model.hpp"
#include "graphics/Material.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Shader.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/VertexBuffer.hpp"
#include "graphics/IndexBuffer.hpp"
#include "graphics/VertexBufferLayout.hpp"
#include "graphics/Window.hpp"

int main() {
    int r = Window::InitWindow(640, 480, "Hello World");
    if (r == -1)
        return r;

    Window::ClearColour = glm::vec3(66.f / 255.f, 133.f / 255.f, 166.f / 255.f);

    Model model(Geometry::PLANE, "../res/base.mat");
    Material* mat = model.GetMaterial();
    mat->SetUniformVec3("uColour", glm::vec3(0, .5f, 0));

    while (!Window::ShouldClose()) {
        /* Execute event results */
        if (Input::IsPressed("esc"))
            glfwSetWindowShouldClose(Window::Frame, GL_TRUE);

        model.Draw();
        Window::Draw();
    }

    Window::DestoryWindow();

    return 0;
}