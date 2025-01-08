#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/shaderProgram.h"
#include "renderer/texture2D.h"
#include "renderer/sprite.h"
#include "resources/resourceManager.h"

using namespace std;
using namespace glm;
using namespace spdlog;

ivec2 g_winSize(640, 480);

// clang-format off
GLfloat points[] = {
    0.0f, 50.f, 0.0f, 
    50.f, -50.f, 0.0f,
    -50.f, -50.f, 0.0f,
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f, 
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

GLfloat texCoords[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
};
// clang-format on

void glfwWinSizeCallback(GLFWwindow* pwindow, int width, int height) {
    g_winSize.x = width;
    g_winSize.y = height;
    glViewport(0, 0, g_winSize.x, g_winSize.y);
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
    pwindow = glfwCreateWindow(g_winSize.x, g_winSize.y, "GameCpp", nullptr, nullptr);
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

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pDefaultShaderProgram) {
            error("[ERROR::Main] Can't crate shader program: {}", "SpriteShader");
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        vector<string> subTexturesNames = {"block",        "topBlock",      "bottomBlock",     "leftBlock",        "rightBlock",
                                           "topLeftBlock", "topRightBlock", "bottomLeftBlock", "bottomRightBlock", "beton"};
        auto pTextureAtlas =
            resourceManager.laodTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", move(subTexturesNames), 16, 16);

        auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "topRightBlock");
        pSprite->setPosition(vec2(300, 100));

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint texCoords_vbo = 0;
        glGenBuffers(1, &texCoords_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoords_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

        GLuint vertex_array_obj = 0;
        glGenVertexArrays(1, &vertex_array_obj);
        glBindVertexArray(vertex_array_obj);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoords_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        mat4 modelMatrix_1 = mat4(1.f);
        modelMatrix_1 = translate(modelMatrix_1, vec3(50.f, 50.f, 0.f));

        mat4 modelMatrix_2 = mat4(1.f);
        modelMatrix_2 = translate(modelMatrix_2, vec3(590.f, 200.f, 0.f));

        mat4 projectMatrix = ortho(0.f, static_cast<float>(g_winSize.x), 0.f, static_cast<float>(g_winSize.y), -100.f, 100.f);

        pDefaultShaderProgram->setMatrix4("projectionMat", projectMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projectionMat", projectMatrix);
        /* Loop until the user closes the window */
        float x, y = 0.0f;
        float i;

        while (!glfwWindowShouldClose(pwindow)) {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vertex_array_obj);
            tex->bind();

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // TODO: удали анимацию
            pSprite->render();
            if (glfwGetKey(pwindow, GLFW_KEY_A) == GLFW_PRESS) {
                x -= 1.0f;
            }
            if (glfwGetKey(pwindow, GLFW_KEY_D) == GLFW_PRESS) {
                x += 1.0f;
            }
            if (glfwGetKey(pwindow, GLFW_KEY_S) == GLFW_PRESS) {
                y -= 1.0f;
            }
            if (glfwGetKey(pwindow, GLFW_KEY_W) == GLFW_PRESS) {
                y += 1.0f;
            }
            if (glfwGetMouseButton(pwindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                i += 1;
            }
            if (glfwGetMouseButton(pwindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                i -= 1;
            }
            pSprite->setRotation(i);
            pSprite->setPosition(vec2(x, y));
            /* Swap front and back buffers */
            glfwSwapBuffers(pwindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}