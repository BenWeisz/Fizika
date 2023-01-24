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
#include "graphics/Light.hpp"
#include "graphics/Transform.hpp"
#include "graphics/gizmo/AxisGizmo.hpp"
#include "simulation/EnergyPlot.hpp"
#include "simulation/Simulation.hpp"
#include "objects/Plane.hpp"
#include "objects/BoundaryCube.hpp"

const int WIDTH = 640 * 1.5;
const int HEIGHT = 480 * 1.5;

int main() {
    {
        Input::RegisterBinding(GLFW_KEY_B, "New Ball");
        Input::RegisterBinding(GLFW_KEY_N, "Toggle Collisions");

        // Set up the window
        int r = Window::InitWindow(WIDTH, HEIGHT, "Fizika - Bouncing Particle Collision", true);
        if (r == -1)
            return r;

        Window::ClearColour = glm::vec3(66.f / 255.f, 133.f / 255.f, 166.f / 255.f);

        // Set up the camera for the window
        Camera::InitCamera(3.0f, WIDTH, HEIGHT);

        Light::InitLight(glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 0.0, 10.0));

        /* Set up Models */
        Plane plane;
        Transform* planeTransform = plane.GetTransform();
        planeTransform->SetRotation(glm::vec3(0.0, 1.0, 0.0), -90.0);
        planeTransform->SetScale(glm::vec3(3.0, 3.0, 3.0));

        BoundaryCube cube;
        Transform* cubeTransform = cube.GetTransform();
        cubeTransform->SetScale(glm::vec3(3.0, 3.0, 3.0));
        cubeTransform->SetTranslation(glm::vec3(0.0, 0.0, 1.5));

        AxisGizmo axis(WIDTH - ((WIDTH / 640) * 40.0), HEIGHT - ((HEIGHT / 480) * 40.0));
        EnergyPlot energies;

        Simulation sim(3.0, 3.0);

        double oldTime = glfwGetTime();

        while (!Window::ShouldClose()) {
            /* Execute event results */
            if (Input::IsPressed("esc"))
                glfwSetWindowShouldClose(Window::Frame, GL_TRUE);

            double currTime = glfwGetTime();
            double dt = currTime - oldTime;
            oldTime = currTime;

            Camera::UpdateCamera();

            sim.Update(dt);

            // Begin creating the frame
            Window::BeginFrame();

            ImGui::Begin("Info");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("Number of Balls: %d", sim.GetNumberOfBalls());
            ImGui::Checkbox("Collision", &sim.mCollisionIsOn);
            ImGui::NewLine();

            energies.AddPoint(sim.GetKineticEnergy(), sim.GetPotentialEnergy());
            energies.Draw();

            ImGui::End();

            /* Render all the objects */
            plane.Draw();
            cube.Draw();
            axis.Draw();
            sim.Draw();

            // Call Draw to actually draw everything
            Window::Draw();
        }
    }

    Window::DestoryWindow();

    return 0;
}