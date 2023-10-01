#include "graphics/Window.hpp"

GLFWwindow* Window::Frame;
std::pair<int, int> Window::Dimensions;
Shader* Window::ShadowShader;
bool Window::HasImGuiDisplay;