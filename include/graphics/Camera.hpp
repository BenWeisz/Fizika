#pragma once

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/Window.hpp"
#include "input/Input.hpp"
#include <GLFW/glfw3.h>

const double MINIMUM_CAMERA_DISTANCE = 3.0;

class Camera {
   private:
    static glm::vec3 CameraPos;
    static float DefaultDistance;
    static float Distance;
    static float Pitch;
    static float Yaw;
    static float LastScreenMouseX;
    static float LastScreenMouseY;
    static glm::mat4 CameraTransform;
    static glm::mat4 ProjectionTransform;
    static bool IsEnabled;

   public:
    Camera() = delete;
    Camera(Camera& other) = delete;
    void operator=(const Camera& other) = delete;
    static void InitCamera(const float defaultDistance, const float width, const float height) {
        DefaultDistance = defaultDistance;
        Distance = DefaultDistance;
        Pitch = 0.0;
        Yaw = 0.0;
        LastScreenMouseX = width / 2.0;
        LastScreenMouseY = height / 2.0;

        ProjectionTransform = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.f);

        Input::SetScroll(std::max(0.0, defaultDistance - MINIMUM_CAMERA_DISTANCE));

        glfwSetInputMode(Window::Frame, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    static void UpdateCamera() {
        if (Input::IsPressed("ui-mode-toggle")) {
            IsEnabled = !IsEnabled;
            if (IsEnabled) {
                glfwSetInputMode(Window::Frame, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                Input::PopInputSave();
            } else {
                glfwSetInputMode(Window::Frame, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                Input::PushInputSave();
            }
        }

        if (IsEnabled) {
            auto mouse = Input::GetMouse();
            double scroll = Input::GetScroll();
            Distance = MINIMUM_CAMERA_DISTANCE + scroll;

            double xScreenOffset = mouse.first - LastScreenMouseX;
            double yScreenOffset = LastScreenMouseY - mouse.second;

            LastScreenMouseX = mouse.first;
            LastScreenMouseY = mouse.second;

            const double sensitivity = 0.1;

            xScreenOffset *= sensitivity;
            yScreenOffset *= sensitivity;

            Pitch -= yScreenOffset;
            Yaw -= xScreenOffset;

            // Clamp so you don't get glitches
            if (Pitch > 44.0f)
                Pitch = 44.0f;
            if (Pitch < -44.0f)
                Pitch = -44.0f;

            // Calculate new camera pos based on the yaw and the pitch
            float x = cos(glm::radians(Yaw));
            float y = sin(glm::radians(Yaw));
            float z = sin(glm::radians(Pitch));

            // Normalize the position to the unit sphere and then set it at Distance units away
            CameraPos = glm::normalize(glm::vec3(x, y, z)) * Distance;

            const glm::vec3 cameraTarget = glm::vec3(0.f, 0.f, 0.f);
            const glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);

            CameraTransform = glm::lookAt(CameraPos, cameraTarget, up);
        }
    }
    static glm::mat4 GetCameraTransform() {
        return CameraTransform;
    }
    static glm::mat4 GetProjectionTransform() {
        return ProjectionTransform;
    }
    static glm::vec3 GetCameraPos() {
        return CameraPos;
    }
    static float GetYaw() {
        return Yaw;
    }
    static float GetPitch() {
        return Pitch;
    }
};