#pragma once

#include "sprite.h"

#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <vector>

using namespace std;
using namespace glm;

namespace Renderer {
class AnimateSprite : public Sprite {
   private:
    map<string, vector<pair<string, uint64_t>>> m_statesMap;
    map<string, vector<pair<string, uint64_t>>>::const_iterator m_pcurrentAnimationDuaration;

    size_t m_currentFrame = 0;
    uint64_t m_currentAnimationTime = 0;

    mutable bool m_dirty = false;

   public:
    AnimateSprite(const shared_ptr<Texture2D> pTexture, const string initialSubTexture, const shared_ptr<ShaderProgram> pShaderProgram,
                  const vec2& posotion = vec2(0.f), const vec2& size = vec2(1.f), const float rotation = 0.f);
    ~AnimateSprite();

    void insertState(string stateName, vector<pair<string, uint64_t>> subTexturesDuaration);

    void render() const override;

    void setState(const string& newState);

    void update(const uint64_t delta);
};
}  // namespace Renderer