#pragma once

#include <string>
#include <iostream>
#include <utility>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <implot.h>
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
    static bool HasImGuiDisplay;
    Window() = delete;
    Window(Window& other) = delete;
    static int InitWindow(const int width, const int height, const std::string& title, const bool hasImGuiDisplay) {
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Create a windowed mode window and its OpenGL context */
        Frame = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!Frame) {
            glfwTerminate();
            return -1;
        }

        // Initialize the input manager
        Input::InitInput(width, height, Frame, hasImGuiDisplay);

        /* Make the window's context current */
        glfwMakeContextCurrent(Frame);
        glfwSwapInterval(1);

        if (hasImGuiDisplay) {
            /* Set up IMGui */
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImPlot::CreateContext();
            ImGui::StyleColorsClassic();

            // Set up the implementation
            ImGui_ImplGlfw_InitForOpenGL(Frame, true);
            ImGui_ImplOpenGL3_Init("#version 450");
        }

        /* Add a simple key listener */
        Input::RegisterBinding(GLFW_KEY_ESCAPE, "esc");
        Input::RegisterBinding(GLFW_KEY_C, "ui-mode-toggle");

        /* Add Scroll Listener */
        glfwSetScrollCallback(Frame, Input::ScrollCallback);

        /* glad: load all OpenGL function pointers */
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        glEnable(GL_DEPTH_TEST);

        Dimensions = std::pair<int, int>(width, height);
        HasImGuiDisplay = hasImGuiDisplay;

        return 0;
    }
    static void DestoryWindow() {
        // Clear up ImGui
        if (HasImGuiDisplay) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            ImPlot::DestroyContext();
        }

        glfwDestroyWindow(Frame);
        glfwTerminate();
    }
    static bool ShouldClose() {
        return glfwWindowShouldClose(Frame);
    }
    static void Draw() {
        // Render ImGui Frame
        if (HasImGuiDisplay) {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(Frame);

        /* Poll for and process events */
        glfwPollEvents();
        glClearColor(ClearColour.x, ClearColour.y, ClearColour.z, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Input::Update();
    }
    static std::pair<int, int> GetDimensions() {
        return Dimensions;
    }
    static void BeginFrame() {
        if (HasImGuiDisplay) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }
    }
};