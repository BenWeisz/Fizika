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
#include "graphics/LightManager.hpp"
#include "graphics/Transform.hpp"
#include "graphics/gizmo/AxisGizmo.hpp"
#include "graphics/FrameBuffer.hpp"
#include "simulation/EnergyPlot.hpp"

#include "objects/Plane.hpp"
#include "objects/BoundaryCube.hpp"
#include "objects/Box.hpp"

const int WIDTH = 640 * 1.5;
const int HEIGHT = 480 * 1.5;

int main() {
    {
        // Set up the window
        glm::vec3 clearColor = glm::vec3(66.f / 255.f, 133.f / 255.f, 166.f / 255.f);
        int r = Window::InitWindow(WIDTH, HEIGHT, "Fizika", true, clearColor);
        if (r == -1)
            return r;

        // Set up the camera for the window
        Camera::InitCamera(-1.0f, WIDTH, HEIGHT);

        LightManager::AddDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(10.0f, 10.0f, 10.0f));

        /* Set up Models */
        Plane plane;
        Transform* planeTransform = plane.GetTransform();
        planeTransform->SetTranslation(glm::vec3(0.0f, 0.0f, -1.5f));

        BoundaryCube cube;
        Transform* cubeTransform = cube.GetTransform();
        cubeTransform->SetScale(glm::vec3(3.0f, 3.0f, 3.0f));

        Box box;
        Transform* boxTransform = box.GetTransform();
        boxTransform->SetScale(glm::vec3(0.25f, 0.25f, 0.25f));
        boxTransform->Translate(glm::vec3(1.0f, 1.0f, 0.0f));

        AxisGizmo axis(WIDTH - ((WIDTH / 640) * 40.0f), HEIGHT - ((HEIGHT / 480) * 40.0f));
        // EnergyPlot energies;

        // Build Shadow Map
        FrameBuffer* shadowBuffer = new FrameBuffer(1024, 1024);
        shadowBuffer->AddAttachment(FrameBuffer::AttachmentType::DEPTH);
        shadowBuffer->Pack();

        // Build Shadow Shader & Uniforms
        DirectionalLight* sunlight = LightManager::GetDirectionalLight(0);

        while (!Window::ShouldClose()) {
            /* Execute event results */
            if (Input::IsPressed("esc"))
                Window::SignalClose();

            Camera::UpdateCamera();

            // Preform updates before render
            // mesh->mPositions(2, 2) += 0.001;
            // mesh->Update();

            // Begin the Shadow Pass
            Window::BeginPass(Window::RenderPassType::SHADOW, shadowBuffer);

            plane.Draw(Window::RenderPassType::SHADOW);

            Window::EndPass(Window::RenderPassType::SHADOW, shadowBuffer);

            // Begin the Opaque Pass
            FrameBuffer* defaultFrameBuffer = FrameBuffer::GetDefaultFrameBuffer();
            Window::BeginPass(Window::RenderPassType::OPAQUE, defaultFrameBuffer);

            glm::vec3 cameraPos = Camera::GetCameraPos();

            ImGui::Begin("Info");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("Camera Position (x, y, z): (%.3f, %.3f, %.3f)", cameraPos[0], cameraPos[1], cameraPos[2]);
            ImGui::NewLine();
            ImGui::End();

            /* Render all the objects */
            plane.Draw(Window::RenderPassType::OPAQUE);
            cube.Draw();
            box.Draw(Window::RenderPassType::OPAQUE);
            axis.Draw();

            // Call Draw to actually draw everything
            Window::EndPass(Window::RenderPassType::OPAQUE, defaultFrameBuffer);
        }

        delete shadowBuffer;
    }

    Window::DestoryWindow();

    return 0;
}