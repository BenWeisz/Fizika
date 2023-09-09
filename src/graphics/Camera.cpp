#include "graphics/Camera.hpp"

glm::vec3 Camera::CameraPos;
float Camera::Distance;
float Camera::DefaultDistance;
float Camera::Pitch;
float Camera::Yaw;
float Camera::LastScreenMouseX;
float Camera::LastScreenMouseY;
glm::mat4 Camera::CameraTransform;
glm::mat4 Camera::ProjectionTransform;
bool Camera::IsEnabled = true;