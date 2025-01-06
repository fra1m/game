#pragma once

#include <glad/glad.h>
#include <iostream>

#include "../renderer/shaderProgram.h"
#include "../resources/resourceManager.h"
#include <spdlog/spdlog.h>

using namespace std;
using namespace spdlog;

namespace Renderer {
class Texture2D {
   private:
    GLuint m_ID;
    GLenum m_mode;
    unsigned int m_width;
    unsigned int m_height;

   public:
    Texture2D(const GLuint width, const GLuint height, const unsigned char* data, const unsigned int channels = 4,
              const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
    ~Texture2D();

    Texture2D() = delete;
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D& operator=(Texture2D&& texture2D);
    Texture2D(Texture2D&& texture2D);
    void bind() const;
};
}  // namespace Renderer
