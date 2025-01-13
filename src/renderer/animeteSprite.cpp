#include "animeteSprite.h"
#include "texture2D.h"

#include <spdlog/spdlog.h>
#include <iostream>
#include <memory>

using namespace spdlog;
using namespace std;
using namespace glm;

namespace Renderer {
AnimateSprite::AnimateSprite(const shared_ptr<Texture2D> pTexture, const string initialSubTexture,
                             const shared_ptr<ShaderProgram> pShaderProgram, const vec2& posotion, const vec2& size, const float rotation)
    : Sprite(move(pTexture), move(initialSubTexture), move(pShaderProgram), posotion, size, rotation) {
    m_pcurrentAnimationDuaration = m_statesMap.end();
};

void AnimateSprite::insertState(string stateName, vector<pair<string, uint64_t>> subTexturesDuaration) {
    m_statesMap.emplace(move(stateName), move(subTexturesDuaration));
};

void AnimateSprite::render() const {
    if (m_dirty) {
        auto subTexture = m_pTexture->getSubTexture(m_pcurrentAnimationDuaration->second[m_currentFrame].first);
        // clang-format off

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
            subTexture.leftBottomUV.x,  subTexture.leftBottomUV.y,
            subTexture.leftBottomUV.x,  subTexture.rightTopUV.y,
            subTexture.rightTopUV.x,    subTexture.rightTopUV.y,

            subTexture.rightTopUV.x,    subTexture.rightTopUV.y,
            subTexture.rightTopUV.x,    subTexture.leftBottomUV.y,
            subTexture.leftBottomUV.x,  subTexture.leftBottomUV.y,
        };
        // clang-format on

        glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordsVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textureCoords), &textureCoords);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        m_dirty = false;
    }

    Sprite::render();
};

void AnimateSprite::setState(const string& newState) {
    auto it = m_statesMap.find(newState);
    if (it != m_statesMap.end()) {
        error("[ERROR::AnimateSprite] can't find animation state");
        return;
    }

    if (it != m_pcurrentAnimationDuaration) {
        m_currentAnimationTime = 0;
        m_currentFrame = 0;

        m_pcurrentAnimationDuaration = it;
        m_dirty = true;
    }
};

void AnimateSprite::update(const uint64_t delta) {
    if (m_pcurrentAnimationDuaration != m_statesMap.end()) {
        m_currentAnimationTime += delta;

        while (m_currentAnimationTime >= m_pcurrentAnimationDuaration->second[m_currentFrame].second) {
            m_currentAnimationTime -= m_pcurrentAnimationDuaration->second[m_currentFrame].second;
            ++m_currentFrame;
            m_dirty = true;
        }

        if (m_currentFrame == m_pcurrentAnimationDuaration->second.size()) {
            m_currentFrame = 0;
        }
    }
};

}  // namespace Renderer
