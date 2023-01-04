#pragma once

#include <unordered_map>
#include <string>
#include <algorithm>
#include <cctype>
#include <utility>
#include <iostream>

#include <GLFW/glfw3.h>

class Input {
   private:
    static std::unordered_map<std::string, bool> KeyStatesPress;
    static std::unordered_map<std::string, bool> KeyStatesReleased;
    static std::unordered_map<std::string, bool> KeyStatesHeld;
    static std::vector<std::pair<int, std::string>> mRegisteredBindings;
    static std::vector<std::string> GetBindingNames(int key) {
        std::vector<std::string> bindingNames;
        for (auto& e : mRegisteredBindings) {
            if (e.first == key)
                bindingNames.push_back(e.second);
        }

        return bindingNames;
    }
    static bool GetState(std::unordered_map<std::string, bool> map, const std::string& bindingName) {
        std::unordered_map<std::string, bool>::const_iterator element =
            map.find(bindingName);

        if (element == map.end()) {
            std::cout << "ERROR: Key " << bindingName << " not registered with input handler\n\tCall RegisterBinding on the global Input instance.";
            return false;
        }

        return element->second;
    }

   public:
    Input() = delete;
    Input(Input& other) = delete;
    void operator=(const Input& other) = delete;
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Get list of binding names that correspond to key code
        std::vector<std::string> bindingNames = GetBindingNames(key);

        // If the key isn't registered ignore it
        if (!bindingNames.size())
            return;

        // Update the key states
        // Updates from press action
        if (action == GLFW_PRESS) {
            for (auto& name : bindingNames) {
                KeyStatesPress[name] = true;
                KeyStatesReleased[name] = false;
            }
        }
        // Updates from release action
        else if (action == GLFW_RELEASE) {
            for (auto& name : bindingNames) {
                KeyStatesReleased[name] = true;
                KeyStatesHeld[name] = false;
            }
        }
        // Updates from repeat action
        else if (action == GLFW_REPEAT) {
            for (auto& name : bindingNames) {
                KeyStatesHeld[name] = true;
                KeyStatesPress[name] = false;
            }
        }
    }
    static void RegisterBinding(int key, const std::string& bindingName) {
        // Register a key so that you can query for it's state later
        mRegisteredBindings.push_back({key, bindingName});
        KeyStatesPress[bindingName] = false;
        KeyStatesReleased[bindingName] = true;
        KeyStatesHeld[bindingName] = false;
    }
    static bool IsPressed(const std::string& bindingName) {
        return GetState(KeyStatesPress, bindingName);
    }
    static bool IsReleased(const std::string& bindingName) {
        return GetState(KeyStatesReleased, bindingName);
    }
    static bool IsHeld(const std::string& bindingName) {
        return GetState(KeyStatesHeld, bindingName);
    }
};
