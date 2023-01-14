#include "input/Input.hpp"

std::unordered_map<std::string, Input::State> Input::KeyStates;
std::vector<std::pair<std::string, int>> Input::RegisteredBindings;
double Input::MouseX;
double Input::MouseY;
double Input::Scroll = 0.0;
GLFWwindow* Input::Frame;
bool Input::HasImGuiDisplay;
std::vector<Input::InputSave> Input::InputSaves;