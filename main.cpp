#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "VertexArray.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

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
    glfwSetKeyCallback(window, key_callback);

    /* glad: load all OpenGL function pointers */
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    {
        Shader shader("../res/base.vert", "../res/base.frag");
        VertexBufferLayout layout(shader);
        layout.AddLayoutElement("iPosition", 2);

        std::vector<GLfloat> data = {
            0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f
        };
        VertexBuffer buffer(data);

        VertexArray vao(buffer, layout);

        // std::vector<GLfloat> data = {1.0f, 2.0f, 3.0f};
        // VertexBuffer* a = new VertexBuffer(data);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            vao.Bind();

            glDrawArrays(GL_TRIANGLES, 0, 3);

            vao.Unbind();
            shader.Unbind();

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}