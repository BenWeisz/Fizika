#pragma once

#include <unordered_map>
#include <string>
#include <algorithm>
#include <cctype>
#include <utility>
#include <iostream>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <glfw_keycodes_to_string.hpp>

class Input {
   public:
    struct InputSave {
        double mouseX;
        double mouseY;
        double scroll;
    };
    enum State {
        IDLE,
        PRESSED,
        HELD,
        RELEASED
    };

   private:
    static std::unordered_map<std::string, State> KeyStates;
    // It is more likely that we want to have multiple keys bound to 1 action
    // the reverse is not as likely because these would be seen as collisions
    static std::vector<std::pair<std::string, int>> RegisteredBindings;
    static double MouseX;
    static double MouseY;
    static double Scroll;
    static GLFWwindow* Frame;
    static bool HasImGuiDisplay;
    static std::vector<InputSave> InputSaves;

    static State GetState(const std::string& bindingName) {
        std::unordered_map<std::string, State>::const_iterator element =
            KeyStates.find(bindingName);

        if (element == KeyStates.end()) {
            std::cout << "ERROR: Key " << bindingName << " not registered with input handler\n\tCall RegisterBinding on the global Input instance." << std::endl;
            return State::IDLE;
        }

        return element->second;
    }

   public:
    Input() = delete;
    Input(Input& other) = delete;
    void operator=(const Input& other) = delete;
    static void InitInput(const double width, const double height, GLFWwindow* frame, const bool hasImGuiDisplay) {
        MouseX = width / 2.0;
        MouseY = height / 2.0;
        Frame = frame;
        HasImGuiDisplay = hasImGuiDisplay;

        std::cout << "Registered key bindings:" << std::endl;
        for (auto& e : RegisteredBindings) {
            const char* keyName = KeyCodeToStringSwitch((KeyCode)e.second);
            std::cout << e.first << ": \"" << keyName << "\"" << std::endl;
        }
    }
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        Scroll -= yoffset;
        Scroll = std::max(0.0, Scroll);

        if (HasImGuiDisplay)
            ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    }
    static void RegisterBinding(int key, const std::string& bindingName) {
        // Register a key so that you can query for it's state later
        RegisteredBindings.push_back({bindingName, key});
        KeyStates[bindingName] = State::IDLE;
    }

    static bool IsIdle(const std::string& bindingName) {
        return GetState(bindingName) == State::IDLE;
    }
    static bool IsPressed(const std::string& bindingName) {
        return GetState(bindingName) == State::PRESSED;
    }
    static bool IsReleased(const std::string& bindingName) {
        return GetState(bindingName) == State::RELEASED;
    }
    static bool IsHeld(const std::string& bindingName) {
        return GetState(bindingName) == State::HELD;
    }

    static std::pair<double, double> GetMouse() {
        return {MouseX, MouseY};
    }
    static double GetScroll() {
        return Scroll;
    }

    static void SetScroll(double scroll) {
        Scroll = scroll;
    }

    static void Update() {
        // The input workhorse function

        // Key Bindings Updates
        for (auto& binding : RegisteredBindings) {
            std::string bindingName = binding.first;
            int key = binding.second;

            int state = glfwGetKey(Frame, key);
            State prevState = KeyStates[bindingName];
            if (state == GLFW_PRESS) {
                if (prevState == State::IDLE)
                    KeyStates[bindingName] = State::PRESSED;
                else if (prevState == State::PRESSED)
                    KeyStates[bindingName] = State::HELD;
            } else if (state == GLFW_RELEASE) {
                if (prevState == State::HELD)
                    KeyStates[bindingName] = State::RELEASED;
                else if (prevState == State::RELEASED)
                    KeyStates[bindingName] = State::IDLE;
            }
        }

        // Mouse Position Updates
        glfwGetCursorPos(Frame, &MouseX, &MouseY);
    }

    static void PushInputSave() {
        InputSaves.push_back({MouseX, MouseY, Scroll});
    }
    static void PopInputSave() {
        if (InputSaves.size() > 0) {
            InputSave save = InputSaves.back();
            MouseX = save.mouseX;
            MouseY = save.mouseY;
            Scroll = save.scroll;
            InputSaves.pop_back();
        }
    }
};
