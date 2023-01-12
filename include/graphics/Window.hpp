#pragma once

#include <string>
#include <iostream>
#include <utility>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "input/Input.hpp"

class Window {
   private:
    static std::pair<int, int> Dimensions;

   public:
    static GLFWwindow* Frame;
    static glm::vec3 ClearColour;
    Window() = delete;
    Window(Window& other) = delete;
    static int InitWindow(const int width, const int height, const std::string& title) {
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Create a windowed mode window and its OpenGL context */
        Input::InitInput(width, height);
        Frame = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!Frame) {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(Frame);

        /* Add a simple key listener */
        Input::RegisterBinding(GLFW_KEY_ESCAPE, "esc");
        glfwSetKeyCallback(Frame, Input::KeyCallback);

        /* Add Mouse Listeners */
        glfwSetCursorPosCallback(Frame, Input::MouseCallback);
        glfwSetScrollCallback(Frame, Input::ScrollCallback);

        /* glad: load all OpenGL function pointers */
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        glEnable(GL_DEPTH_TEST);

        Dimensions = std::pair<int, int>(width, height);

        return 0;
    }
    static void DestoryWindow() {
        glfwTerminate();
    }
    static bool ShouldClose() {
        return glfwWindowShouldClose(Frame);
    }
    static void Draw() {
        /* Swap front and back buffers */
        glfwSwapBuffers(Frame);

        /* Poll for and process events */
        glfwPollEvents();
        glClearColor(ClearColour.x, ClearColour.y, ClearColour.z, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    static std::pair<int, int> GetDimensions() {
        return Dimensions;
    }
};