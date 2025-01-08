#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

using namespace std;

namespace Renderer {
class ShaderProgram;
class Texture2D;
class Sprite;
}  // namespace Renderer

class ResourceManager {
   private:
    string getFileString(const string& relativeFilePath) const;

    typedef map<const string, shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
    ShaderProgramsMap m_shaderPrograms;

    typedef map<const string, shared_ptr<Renderer::Texture2D>> TextureMap;
    TextureMap m_textures;

    typedef map<const string, shared_ptr<Renderer::Sprite>> SpritesMap;
    SpritesMap m_sprites;

    string m_path;

   public:
    ResourceManager(const string& execPath);
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;
    ResourceManager(ResourceManager&&) = delete;

    shared_ptr<Renderer::ShaderProgram> loadShaders(const string& shaderName, const string& vertexPath, const string& fragmentPath);
    shared_ptr<Renderer::ShaderProgram> getShaderProgram(const string& shaderName);

    shared_ptr<Renderer::Texture2D> loadTexture(const string& textureName, const string& texturePath);
    shared_ptr<Renderer::Texture2D> getTexture(const string& textureName);

    shared_ptr<Renderer::Sprite> loadSprite(const string& spriteName, const string& textureName, const string& shaderName,
                                            const unsigned int spriteWidth, const unsigned int spriteHeight, const string& subTextureName = "default");
    shared_ptr<Renderer::Sprite> getSprite(const string& spriteName);

    shared_ptr<Renderer::Texture2D> laodTextureAtlas(const string textureName, const string texturePath, const vector<string> subTextures,
                                                     const unsigned int subTextureWidth, const unsigned int subTextureHeight);
};