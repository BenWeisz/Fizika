#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "graphics/Window.hpp"
#include "input/Input.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Model.hpp"
#include "graphics/Material.hpp"
#include "graphics/gizmo/AxisGizmo.hpp"

int main() {
    {
        // Set up the window
        int r = Window::InitWindow(640, 480, "Hello World");
        if (r == -1)
            return r;

        Window::ClearColour = glm::vec3(66.f / 255.f, 133.f / 255.f, 166.f / 255.f);

        // Set up the camera for the window
        Camera::InitCamera(3.0f, 640, 480);

        Model model(Geometry::CUBE, "../res/base.mat");
        Material* mat = model.GetMaterial();
        mat->SetUniformVec3("uColour", glm::vec3(0, .5f, .2f));

        AxisGizmo g(100.0, 100.0);

        while (!Window::ShouldClose()) {
            /* Execute event results */
            if (Input::IsPressed("esc"))
                glfwSetWindowShouldClose(Window::Frame, GL_TRUE);

            Camera::UpdateCamera();

            mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
            mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
            mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);

            // model.Draw();
            g.Draw();

            // Call Draw to actually draw everything
            Window::Draw();
        }
    }

    Window::DestoryWindow();

    return 0;
}