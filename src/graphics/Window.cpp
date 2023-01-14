#include "graphics/Window.hpp"

GLFWwindow* Window::Frame;
glm::vec3 Window::ClearColour;
std::pair<int, int> Window::Dimensions;
bool Window::HasImGuiDisplay;