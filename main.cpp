#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/Window.hpp"
#include "input/Input.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Model.hpp"
#include "graphics/Material.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"
#include "graphics/gizmo/AxisGizmo.hpp"
#include "simulation/EnergyPlot.hpp"

const int WIDTH = 640 * 1.5;
const int HEIGHT = 480 * 1.5;

int main() {
    {
        // Set up the window
        int r = Window::InitWindow(WIDTH, HEIGHT, "Fizika", true);
        if (r == -1)
            return r;

        Window::ClearColour = glm::vec3(66.f / 255.f, 133.f / 255.f, 166.f / 255.f);

        // Set up the camera for the window
        Camera::InitCamera(3.0f, WIDTH, HEIGHT);

        /* Set up the models */
        Texture checker("../res/textures/checker.png");

        Model model(Geometry::PLANE, "../res/base-texture.mat");
        Mesh* mesh = model.GetMesh();
        Material* mat = model.GetMaterial();
        // mat->SetUniformVec3("uFlatColour", glm::vec3(0.0, 1.0, 0.0));
        mat->SetUniformVec3("uLightColour", glm::vec3(1.0));
        mat->SetUniformVec3("uLightPos", glm::vec3(10.0, 10.0, 10.0));
        mat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
        mat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
        mat->AddTexture("uTexture0", &checker);

        glm::mat4 uModel = glm::mat4(1.0);
        uModel = glm::rotate(uModel, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
        uModel = glm::scale(uModel, glm::vec3(3.0, 3.0, 3.0));
        mat->SetUniformMat4("uModel", uModel, false);

        /* Set up Axis Gizmo */
        AxisGizmo axis(WIDTH - ((WIDTH / 640) * 40.0), HEIGHT - ((HEIGHT / 480) * 40.0));
        EnergyPlot energies;

        while (!Window::ShouldClose()) {
            /* Execute event results */
            if (Input::IsPressed("esc"))
                glfwSetWindowShouldClose(Window::Frame, GL_TRUE);

            Camera::UpdateCamera();

            // Preform updates before render
            // mesh->mPositions(2, 2) += 0.001;
            // mesh->Update();

            // Begin creating the frame
            Window::BeginFrame();

            // bool demoIsOpen = true;
            // ImGui::ShowDemoWindow(&demoIsOpen);
            // ImPlot::ShowDemoWindow();
            energies.AddPoint(10.0, 10.0 + glfwGetTime());
            energies.Draw();

            mat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
            mat->SetUniformVec3("uCameraPos", Camera::GetCameraPos());

            // Render everything
            model.Draw();
            axis.Draw();

            // Call Draw to actually draw everything
            Window::Draw();
        }
    }

    Window::DestoryWindow();

    return 0;
}