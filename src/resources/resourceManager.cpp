#include "resourceManager.h"
#include "../renderer/shaderProgram.h"
#include "../renderer/texture2D.h"
#include "../renderer/sprite.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <spdlog/spdlog.h>
#include <glm/vec2.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

using namespace std;
using namespace spdlog;
using namespace glm;

ResourceManager::ResourceManager(const string& execPath) {
    size_t found = execPath.find_last_of("/\\");
    m_path = execPath.substr(0, found);

    info("[INFO::ResourceManager] ResourceManager initialized with m_path: {}", m_path);
};

string ResourceManager::getFileString(const string& relativeFilePath) const {
    ifstream f;
    f.open(m_path + "/" + relativeFilePath, ios::in | ios::binary);
    if (!f.is_open()) {
        error("[ERROR::ResourceManager] Faild to open file: {}", m_path + "/" + relativeFilePath);
        return "";
    }
    stringstream buffer;
    buffer << f.rdbuf();

    return buffer.str();
};

shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const string& shaderName, const string& vertexPath,
                                                                 const string& fragmentPath) {
    string vertexString = getFileString(vertexPath);
    if (vertexString.empty()) {
        error("[ERROR::ResourceManager] No vertex shader!");
        return nullptr;
    }

    string fragmentString = getFileString(fragmentPath);
    if (fragmentString.empty()) {
        error("[ERROR::ResourceManager] No fragment shader!");
        return nullptr;
    }
    shared_ptr<Renderer::ShaderProgram>& newShader =
        m_shaderPrograms.emplace(shaderName, make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
    if (newShader->isCompiled()) {
        return newShader;
    }
    error("[ERROR::ResourceManager] Can't load shader program:\nVertex:{}\nFragment:{}", vertexPath, fragmentPath);
    return nullptr;
};

shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const string& shaderName) {
    ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
    if (it != m_shaderPrograms.end()) {
        return it->second;
    }
    error("[ERROR::ResourceManager] Can't find shader program: {}", shaderName);
    return nullptr;
};

shared_ptr<Renderer::Texture2D> ResourceManager::loadTexture(const string& textureName, const string& texturePath) {
    int channels = 0;
    int width = 0;
    int height = 0;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixeles = stbi_load(string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);
    if (!pixeles) {
        error("[ERROR::ResourceManager] Can't load image: {}, path: {}", textureName, (m_path + "/" + texturePath).c_str());
        return nullptr;
    }

    info("[INFO::ResourceManager] Texture:\nTexture channels: {}\nTexture width: {}\nTexture height: {}\nTexture path: {}", channels, width,
         height, string(m_path + "/" + texturePath).c_str());

    shared_ptr<Renderer::Texture2D> newTexture =
        m_textures.emplace(textureName, make_shared<Renderer::Texture2D>(width, height, pixeles, channels, GL_NEAREST, GL_CLAMP_TO_EDGE))
            .first->second;

    stbi_image_free(pixeles);

    return newTexture;
}

shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const string& textureName) {
    TextureMap::const_iterator it = m_textures.find(textureName);
    if (it != m_textures.end()) {
        return it->second;
    }
    error("[ERROR::ResourceManager] Can't find textures: {}", textureName);
    return nullptr;
};

shared_ptr<Renderer::Sprite> ResourceManager::loadSprite(const string& spriteName, const string& textureName, const string& shaderName,
                                                         const unsigned int spriteWidth, const unsigned int spriteHeight,
                                                         const string& subTextureName) {
    auto pTexture = getTexture(textureName);
    if (!pTexture) {
        error("[ERROR::ResourceManager] Can't find textures: {} for the sprite: {}", textureName, spriteName);
    }

    auto pShader = getShaderProgram(shaderName);
    if (!pShader) {
        error("[ERROR::ResourceManager] Can't find shader: {} for the sprite: {}", shaderName, spriteName);
    }

    shared_ptr<Renderer::Sprite> newSprite =
        m_sprites
            .emplace(textureName,
                     make_shared<Renderer::Sprite>(pTexture, subTextureName, pShader, vec2(0.f, 0.f), vec2(spriteWidth, spriteHeight)))
            .first->second;

    return newSprite;
};

shared_ptr<Renderer::Sprite> ResourceManager::getSprite(const string& spriteName) {
    SpritesMap::const_iterator it = m_sprites.find(spriteName);
    if (it != m_sprites.end()) {
        return it->second;
    }
    error("[ERROR::ResourceManager] Can't find sprite: {}", spriteName);
    return nullptr;
};

shared_ptr<Renderer::Texture2D> ResourceManager::laodTextureAtlas(const string textureName, const string texturePath,
                                                                  const vector<string> subTextures, const unsigned int subTextureWidth,
                                                                  const unsigned int subTextureHeight) {
    auto pTexture = loadTexture(move(textureName), move(texturePath));
    if (pTexture) {
        const unsigned int textureWidth = pTexture->width();
        const unsigned int textureHeight = pTexture->height();
        unsigned int currentTextureOffsetX = 0;
        unsigned int currentTextureOffsetY = textureHeight;

        for (const auto& currentSubTextureName : subTextures) {
            vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth,
                              static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
            vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth,
                            static_cast<float>(currentTextureOffsetY) / textureHeight);
            pTexture->addSubTexture(move(currentSubTextureName), leftBottomUV, rightTopUV);

            currentTextureOffsetX += subTextureWidth;
            if (currentTextureOffsetX >= textureWidth) {
                currentTextureOffsetX = 0;
                currentTextureOffsetY -= subTextureHeight;
            }
        }
    };

    return pTexture;
};