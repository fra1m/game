#pragma once

#include <glad/glad.h>
#include <iostream>

namespace Renderer {
class ShaderProgram {
   public:
    ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
    ~ShaderProgram();
    bool isCompiled() const { return m_isCompiled; }
    void use() const;

    ShaderProgram() = delete;
    ShaderProgram(ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    ShaderProgram& operator=(ShaderProgram&& ShaderProgram);
    ShaderProgram(ShaderProgram&& ShaderProgram);

   private:
    bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
    bool m_isCompiled = false;
    GLuint m_ID = 0;
};
}  // namespace Renderer
