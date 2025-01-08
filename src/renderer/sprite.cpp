#include "Sprite.h"
#include "shaderProgram.h"
#include "texture2D.h"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <spdlog/spdlog.h>
#include <iostream>
#include <memory>

using namespace spdlog;
using namespace std;
using namespace glm;

namespace Renderer {
Sprite::Sprite(const shared_ptr<Texture2D> pTexture, const shared_ptr<ShaderProgram> pShaderProgram, const vec2& posotion, const vec2& size,
               const float rotation)
    : m_pTexture(move(pTexture)), m_pShaderProgram(move(pShaderProgram)), m_posotion(posotion), m_size(size), m_rotation(rotation) {
    // clang-format off
    /** 
     * @brief Vertex coordinates for a quad (X, Y)
     * Organized as two triangles:
     * ```
     *        2--3    1
     *        | /   / |
     *        1    3--2
     * ```
     * - Triangle 1: bottom-left, top-left, top-right 
     *    
     * - Triangle 2: top-right, bottom-right, bottom-left 
     */
    const GLfloat vertexCoords[] = {
        0.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,

        1.f, 1.f,
        1.f, 0.f,
        0.f, 0.f,
    };
    /** 
     * @brief Texture coordinates for a quad (U, V) 
     * 
     * - Texture coordinates correspond to vertex coordinates:
     * ```
     *        2--3    1
     *        | /   / |
     *        1    3--2
     * ``` 
     */
    const GLfloat textureCoords[] = {
        0.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,

        1.f, 1.f,
        1.f, 0.f,
        0.f, 0.f,
    };
    // clang-format on

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_vertexCoordsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), &vertexCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &m_textureCoordsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), &textureCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
};
Sprite::~Sprite() {
    glDeleteBuffers(1, &m_vertexCoordsVBO);
    glDeleteBuffers(1, &m_textureCoordsVBO);
    glDeleteVertexArrays(1, &m_VAO);
};

void Sprite::render() const {
    m_pShaderProgram->use();

    mat4 model(1.f);

    model = translate(model, vec3(m_posotion, 0.f));
    model = translate(model, vec3(0.5 * m_size.x, 0.5 * m_size.y, 0.f));
    model = rotate(model, radians(m_rotation), vec3(0.f, 0.f, 1.f));
    model = translate(model, vec3(-0.5 * m_size.x, -0.5 * m_size.y, 0.f));
    model = scale(model, vec3(m_size, 1.f));

    glBindVertexArray(m_VAO);
    m_pShaderProgram->setMatrix4("modelMat", model);

    glActiveTexture(GL_TEXTURE0);
    m_pTexture->bind();

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
};
void Sprite::setPosition(const vec2& posotion) { m_posotion = posotion; };

void Sprite::setSize(const vec2& size) { m_size = size; };

void Sprite::setRotation(const float rotation) { m_rotation = rotation; };
}  // namespace Renderer
