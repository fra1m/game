#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <iostream>

using namespace std;
using namespace glm;

namespace Renderer {
class ShaderProgram {
   public:
    ShaderProgram(const string& vertexShader, const string& fragmentShader);
    ~ShaderProgram();
    bool isCompiled() const { return m_isCompiled; }
    void use() const;
    void setInt(const string& name, const GLint value);
    void setMatrix4(const string& name, const mat4& matrix);

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
