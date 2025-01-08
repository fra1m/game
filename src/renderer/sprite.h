#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <iostream>
#include <memory>

using namespace std;
using namespace glm;

namespace Renderer {
class Texture2D;
class ShaderProgram;

class Sprite {
   private:
    shared_ptr<Texture2D> m_pTexture;
    shared_ptr<ShaderProgram> m_pShaderProgram;
    vec2 m_posotion;
    vec2 m_size;
    float m_rotation;
    GLuint m_VAO;
    GLuint m_vertexCoordsVBO;
    GLuint m_textureCoordsVBO;

   public:
    Sprite(const shared_ptr<Texture2D> pTexture, const shared_ptr<ShaderProgram> pShaderProgram, const vec2& posotion = vec2(0.f),
           const vec2& size = vec2(1.f), const float rotation = 0.f);
    ~Sprite();

    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;

    void render() const;
    void setPosition(const vec2& posotion);
    void setSize(const vec2& size);
    void setRotation(const float rotation);
};

}  // namespace Renderer