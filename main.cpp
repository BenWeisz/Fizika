#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "graphics/Material.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Shader.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/VertexBuffer.hpp"
#include "graphics/IndexBuffer.hpp"
#include "graphics/VertexBufferLayout.hpp"

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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    {
        Material mat("../res/base.mat");
        Shader* shader = mat.GetShader();
        shader->Bind();
        shader->SetUniformVec3("uColour", glm::vec3(0, .5f, 0));
        shader->Unbind();

        VertexBufferLayout* layout = mat.GetVertexBufferLayout();

        std::vector<GLfloat> data = {
            0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};
        VertexBuffer buffer(data);

        std::vector<GLuint> indexData = {
            0, 1, 2};
        IndexBuffer indexBuffer(indexData);

        VertexArray vao;
        vao.Bundle(&buffer, &indexBuffer, layout, shader);

        // Mesh m(Mesh::Geometry::LINE);
        // m.LoadFromFile("../res/models/triangle.obj");

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader->Bind();
            vao.Bind();

            // glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_INT, (void*)0);

            vao.Unbind();
            shader->Unbind();

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}