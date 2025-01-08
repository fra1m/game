#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <iostream>
#include <map>

#include <spdlog/spdlog.h>

using namespace std;
using namespace glm;
using namespace spdlog;

namespace Renderer {
class Texture2D {
   public:
    struct SubTexture2D {
        vec2 leftBottomUV;
        vec2 rightTopUV;

        SubTexture2D(const vec2& _leftBottomUV, const vec2& _rightTopUV) : leftBottomUV(_leftBottomUV), rightTopUV(_rightTopUV) {};

        SubTexture2D() : leftBottomUV(0.f), rightTopUV(1.f) {};
    };

    Texture2D(const GLuint width, const GLuint height, const unsigned char* data, const unsigned int channels = 4,
              const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
    ~Texture2D();

    Texture2D() = delete;
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D& operator=(Texture2D&& texture2D);
    Texture2D(Texture2D&& texture2D);

    void addSubTexture(const string name, const vec2 leftBottomUV, const vec2 rightTopUV);
    const SubTexture2D& getSubTexture(const string& name) const;
    unsigned int width() const { return m_width; };
    unsigned int height() const { return m_height; };

    void bind() const;

   private:
    GLuint m_ID;
    GLenum m_mode;
    unsigned int m_width;
    unsigned int m_height;

    map<string, SubTexture2D> m_subTextures;
};
}  // namespace Renderer
