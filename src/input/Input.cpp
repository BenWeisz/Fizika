#include "input/Input.hpp"

std::unordered_map<std::string, bool> Input::KeyStatesPress;
std::unordered_map<std::string, bool> Input::KeyStatesReleased;
std::unordered_map<std::string, bool> Input::KeyStatesHeld;
std::vector<std::pair<int, std::string>> Input::mRegisteredBindings;