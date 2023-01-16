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
#include "simulation/Ball.hpp"
#include "simulation/Cube.hpp"

const int WIDTH = 640 * 2.0;
const int HEIGHT = 480 * 2.0;

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

        Model plane(Geometry::PLANE, "../res/base-texture.mat");
        Mesh* planeMesh = plane.GetMesh();
        Material* planeMat = plane.GetMaterial();
        // mat->SetUniformVec3("uFlatColour", glm::vec3(0.0, 1.0, 0.0));
        planeMat->SetUniformVec3("uLightColour", glm::vec3(1.0));
        planeMat->SetUniformVec3("uLightPos", glm::vec3(10.0, 10.0, 10.0));
        planeMat->SetUniformMat4("uProjection", Camera::GetProjectionTransform(), false);
        planeMat->SetUniformMat4("uCorrection", Camera::GetCorrectionTransform(), false);
        planeMat->AddTexture("uTexture0", &checker);

        glm::mat4 uModelPlane = glm::mat4(1.0);
        uModelPlane = glm::rotate(uModelPlane, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
        uModelPlane = glm::scale(uModelPlane, glm::vec3(3.0, 3.0, 3.0));
        planeMat->SetUniformMat4("uModel", uModelPlane, false);

        /* Set up Axis Gizmo */
        AxisGizmo axis(WIDTH - ((WIDTH / 640) * 40.0), HEIGHT - ((HEIGHT / 480) * 40.0));
        EnergyPlot energies;

        Eigen::Vector3d q0(0.0, 0.0, 2.0);
        Eigen::Vector3d qdot0(3.0, 4.0, 2.0);
        Ball ball(q0, qdot0);

        Cube cube(glm::vec3(0.0, 0.0, 1.5));

        double time = glfwGetTime();

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
            ImGui::Begin("Info");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::NewLine();

            // Update the ball
            double currTime = glfwGetTime();
            double dt = currTime - time;
            time = currTime;
            ball.Update(dt);

            // Display the ball's energy
            double potentialEnergy = ball.GetPotentialEnergy();
            double kineticEnergy = ball.GetKineticEnergy();

            energies.AddPoint(kineticEnergy, potentialEnergy);
            energies.Draw();
            ImGui::End();

            // Render the plane
            planeMat->SetUniformMat4("uCamera", Camera::GetCameraTransform(), false);
            planeMat->SetUniformVec3("uCameraPos", Camera::GetCameraPos());
            plane.Draw();

            // Render the ball
            ball.Draw();

            // Render the cube
            cube.Draw();

            // Render the gizmo
            axis.Draw();

            // Call Draw to actually draw everything
            Window::Draw();
        }
    }

    Window::DestoryWindow();

    return 0;
}