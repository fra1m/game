#pragma once

#include <glad/glad.h>
#include <iostream>

using namespace std;

namespace Renderer {
class ShaderProgram {
   public:
    ShaderProgram(const string& vertexShader, const string& fragmentShader);
    ~ShaderProgram();
    bool isCompiled() const { return m_isCompiled; }
    void use() const;

    ShaderProgram() = delete;
    ShaderProgram(ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    ShaderProgram& operator=(ShaderProgram&& ShaderProgram);
    ShaderProgram(ShaderProgram&& ShaderProgram);

   private:
    bool createShader(const string& source, const GLenum shaderType, GLuint& shaderID);
    bool m_isCompiled = false;
    GLuint m_ID = 0;
};
}  // namespace Renderer
