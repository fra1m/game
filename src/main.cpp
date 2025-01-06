#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "renderer/shaderProgram.h"
#include "resources/resourceManager.h"
#include <spdlog/spdlog.h>

using namespace std;
using namespace spdlog;

int g_winSizeX = 640;
int g_winSizeY = 480;

// clang-format off
GLfloat points[] = {
    0.0f, 0.5f, 0.0f, 
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f, 
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};
// clang-format on

void glfwWinSizeCallback(GLFWwindow* pwindow, int width, int height) {
    g_winSizeX = width;
    g_winSizeY = height;
    glViewport(0, 0, g_winSizeX, g_winSizeY);
}

void glfwKeyCallback(GLFWwindow* pwindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pwindow, GL_TRUE);
    }
}

int main(int argc, char** argv) {
    GLFWwindow* pwindow;

    /* Initialize the library */
    if (!glfwInit()) {
        error("[ERROR::Main] FAILED glfwInit", 1);
        return -1;
    }

    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    pwindow = glfwCreateWindow(g_winSizeX, g_winSizeY, "GameCpp", nullptr, nullptr);
    if (!pwindow) {
        error("[ERROR::Main] FAILED glfwCreateWindow", 1);
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pwindow, glfwWinSizeCallback);
    glfwSetKeyCallback(pwindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pwindow);

    if (!gladLoadGL()) {
        error("[ERROR::Main] ERR GLAD", 1);
        return -1;
    }

    info("[INFO::Main] Render: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    info("[INFO::Main] OpenGL version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    glClearColor(1, 1, 0, 1);

    {
        ResourceManager resourceManager(argv[0]);

        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram) {
            error("[ERROR::Main] Can't crate shader program: {}", "DefaultShader");
            return -1;
        }

        resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint vertex_array_obj = 0;
        glGenVertexArrays(1, &vertex_array_obj);
        glBindVertexArray(vertex_array_obj);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pwindow)) {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vertex_array_obj);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            /* Swap front and back buffers */
            glfwSwapBuffers(pwindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}