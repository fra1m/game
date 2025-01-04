#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int g_winSizeX = 640;
int g_winSizeY = 480;

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

int main(void) {
    GLFWwindow* pwindow;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "FAILED glfwInit" << std::endl;
        return -1;
    }

    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    pwindow = glfwCreateWindow(g_winSizeX, g_winSizeY, "GameCpp", nullptr, nullptr);
    if (!pwindow) {
        std::cout << "FAILED glfwCreateWindow" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pwindow, glfwWinSizeCallback);
    glfwSetKeyCallback(pwindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pwindow);

    if (!gladLoadGL()) {
        std::cout << "ERR GLAD" << std::endl;
        return -1;
    }

    std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(1, 1, 0, 1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pwindow)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(pwindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}