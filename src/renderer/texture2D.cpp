#include "texture2D.h"
#include <spdlog/spdlog.h>
#include <glm/vec2.hpp>
#include <iostream>

using namespace std;
using namespace spdlog;
using namespace glm;

// TODO: поиграть с фильтрами и врапами (learn opengl)
namespace Renderer {
Texture2D::Texture2D(const GLuint width, const GLuint height, const unsigned char* data, const unsigned int channels, const GLenum filter,
                     const GLenum wrapMode)
    : m_width(width), m_height(height) {
    switch (channels) {
        case 4:
            m_mode = GL_RGBA;
            break;
        case 3:
            m_mode = GL_RGB;
            break;
        default:
            m_mode = GL_RGBA;
            break;
    }

    glGenTextures(1, &m_ID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
};

Texture2D::~Texture2D() { glDeleteTextures(1, &m_ID); };

Texture2D& Texture2D::operator=(Texture2D&& texture2D) {
    glDeleteTextures(1, &m_ID);
    m_ID = texture2D.m_ID;
    texture2D.m_ID = 0;
    m_mode = texture2D.m_mode;
    m_width = texture2D.m_width;
    m_height = texture2D.m_height;

    return *this;
};

Texture2D::Texture2D(Texture2D&& texture2D) {
    m_ID = texture2D.m_ID;
    texture2D.m_ID = 0;
    m_mode = texture2D.m_mode;
    m_width = texture2D.m_width;
    m_height = texture2D.m_height;
};

void Texture2D::bind() const { glBindTexture(GL_TEXTURE_2D, m_ID); };

void Texture2D::addSubTexture(const string name, const vec2 leftBottomUV, const vec2 rightTopUV) {
    m_subTextures.emplace(move(name), SubTexture2D(leftBottomUV, rightTopUV));
};

const Texture2D::SubTexture2D& Texture2D::getSubTexture(const string& name) const {
    auto it = m_subTextures.find(name);

    if (it != m_subTextures.end()) {
        return it->second;
    }
    const static SubTexture2D defaultSubTexture;
    return defaultSubTexture;
};
}  // namespace Renderer