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
#include "graphics/FrameBuffer.hpp"
#include "graphics/Shader.hpp"

class Window {
   private:
    static std::pair<int, int> Dimensions;
    static Shader* ShadowShader;

   public:
    enum RenderPassType {
        OPAQUE,
        SHADOW
    };

    static GLFWwindow* Frame;
    static bool HasImGuiDisplay;
    Window() = delete;
    Window(Window& other) = delete;
    static int InitWindow(const int width, const int height, const std::string& title, const bool hasImGuiDisplay, const glm::vec3& clearColor) {
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

        /* Make the window's context current */
        glfwMakeContextCurrent(Frame);
        glfwSwapInterval(1);

        /* glad: load all OpenGL function pointers */
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        glEnable(GL_CULL_FACE);

        // Print Versions
        const GLubyte* glVersionString = glGetString(GL_VERSION);
        std::cout << "Rendering Via: OpenGL " << glVersionString << std::endl;

        // Initialize the input manager
        Input::RegisterBinding(GLFW_KEY_ESCAPE, "esc");
        Input::RegisterBinding(GLFW_KEY_C, "ui-mode-toggle");
        Input::InitInput(width, height, Frame, hasImGuiDisplay);

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

        /* Add Scroll Listener */
        glfwSetScrollCallback(Frame, Input::ScrollCallback);

        /* Set up the default FrameBuffer */
        FrameBuffer* defaultFrameBuffer = FrameBuffer::GetDefaultFrameBuffer();
        defaultFrameBuffer->mID = 0;
        defaultFrameBuffer->mWidth = width;
        defaultFrameBuffer->mHeight = height;
        defaultFrameBuffer->mHasColorBuffer = true;
        defaultFrameBuffer->mClearColor = clearColor;
        defaultFrameBuffer->mHasDepthBuffer = true;
        defaultFrameBuffer->mHasStencilBuffer = false;

        Dimensions = std::pair<int, int>(width, height);
        HasImGuiDisplay = hasImGuiDisplay;

        ShadowShader = new Shader("../res/shaders/shadow.vert", "../res/shaders/shadow.frag");

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

        delete FrameBuffer::DefaultFrameBuffer;
        delete ShadowShader;

        glfwDestroyWindow(Frame);
        glfwTerminate();
    }
    static bool ShouldClose() {
        return glfwWindowShouldClose(Frame);
    }
    static void SignalClose() {
        glfwSetWindowShouldClose(Frame, GL_TRUE);
    }
    static std::pair<int, int> GetDimensions() {
        return Dimensions;
    }
    static void BeginPass(const Window::RenderPassType renderPassType, FrameBuffer* frameBuffer) {
        frameBuffer->BeginDraw();

        if (renderPassType == Window::RenderPassType::SHADOW) {
        } else if (renderPassType == Window::RenderPassType::OPAQUE) {
            if (HasImGuiDisplay) {
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
            }
        }
    }
    static void EndPass(const Window::RenderPassType renderPassType, FrameBuffer* frameBuffer) {
        if (renderPassType == Window::RenderPassType::SHADOW) {
        } else if (renderPassType == Window::RenderPassType::OPAQUE) {
            // Render ImGui Frame
            if (HasImGuiDisplay) {
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }

            /* Unbind the frame buffer */
            frameBuffer->EndDraw();

            /* Swap front and back buffers */
            glfwSwapBuffers(Frame);

            /* Poll for and process events */
            glfwPollEvents();

            Input::Update();
        }
    }

    static Shader* GetShadowShader() {
        return ShadowShader;
    }
};