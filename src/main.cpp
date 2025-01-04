#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

const char* vertex_shader =
    "#version 460\n"
    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_color;"
    "out vec3 color;"
    "void main() {"
    "   color = vertex_color;"
    "   gl_Position = vec4(vertex_position, 1.0);"
    "}";

const char* fragment_shader =
    "#version 460\n"
    "in vec3 color;"
    "out vec4 frag_color;"
    "void main() {"
    "   frag_color = vec4(color, 1.0);"
    "}";

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

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);

    glDeleteShader(vs);
    glDeleteShader(fs);

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

        glUseProgram(shader_program);
        glBindVertexArray(vertex_array_obj);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(pwindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}