#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "graphics/Model.hpp"
#include "graphics/Material.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Shader.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/VertexBuffer.hpp"
#include "graphics/IndexBuffer.hpp"
#include "graphics/VertexBufferLayout.hpp"
#include "input/Input.hpp"

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, GL_TRUE);

//     if (key == GLFW_KEY_A && action == GLFW_PRESS)
//         std::cout << "Press A"
//                   << std::endl;
//     if (key == GLFW_KEY_A && action == GLFW_RELEASE)
//         std::cout << "Release A"
//                   << std::endl;
//     if (key == GLFW_KEY_A && action == GLFW_REPEAT)
//         std::cout << "Press A"
//                   << std::endl;
// }

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Add a simple key listener */
    Input::RegisterBinding(GLFW_KEY_ESCAPE, "esc");
    glfwSetKeyCallback(window, Input::KeyCallback);

    /* glad: load all OpenGL function pointers */
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    {
        Model model(Geometry::PLANE, "../res/base.mat");
        Material* mat = model.GetMaterial();
        mat->SetUniformVec3("uColour", glm::vec3(0, .5f, 0));

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Execute event results */
            if (Input::IsPressed("esc"))
                glfwSetWindowShouldClose(window, GL_TRUE);

            /* Render here */
            glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            model.Draw();

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}