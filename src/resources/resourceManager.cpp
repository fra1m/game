#include "resourceManager.h"
#include "../renderer/shaderProgram.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <spdlog/spdlog.h>

using namespace std;
using namespace spdlog;

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
    info("[INFO::ResourceManager] Buffer :\n{}", buffer.str());

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
