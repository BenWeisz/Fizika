#include "graphics/Camera.hpp"

glm::vec3 Camera::CameraPos;
float Camera::Distance;
float Camera::Pitch;
float Camera::Yaw;
float Camera::LastMouseX;
float Camera::LastMouseY;
glm::mat4 Camera::CorrectionTransform;
glm::mat4 Camera::CameraTransform;
glm::mat4 Camera::ProjectionTransform;
bool Camera::IsEnabled = true;