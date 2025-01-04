#include "shaderProgram.h"
#include <iostream>

using namespace std;

namespace Renderer {
ShaderProgram::ShaderProgram(const string& vertexShader, const string& fragmentShader) {
    GLuint vertexShaderID;
    if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) {
        cerr << "VERTEX SHADER Compile-time error:\n" << endl;
        return;
    }

    GLuint fragmentShaderID;
    if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) {
        cerr << "FRAGMENT SHADER Compile-time error:\n" << endl;
        glDeleteShader(vertexShaderID);
        return;
    }

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShaderID);
    glAttachShader(m_ID, fragmentShaderID);
    glLinkProgram(m_ID);

    GLint susccess;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &susccess);
    if (!susccess) {
        GLchar infolog[1024];
        glGetShaderInfoLog(m_ID, 1024, nullptr, infolog);
        cerr << "ERRO::SHADER Link-time error:\n" << infolog << endl;
    } else {
        m_isCompiled = true;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}
bool ShaderProgram::createShader(const string& source, const GLenum shaderType, GLuint& shaderID) {
    shaderID = glCreateShader(shaderType);
    const char* code = source.c_str();
    glShaderSource(shaderID, 1, &code, nullptr);
    glCompileShader(shaderID);

    GLint susccess;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &susccess);
    if (!susccess) {
        GLchar infolog[1024];
        glGetShaderInfoLog(shaderID, 1024, nullptr, infolog);
        cerr << "ERRO::SHADER Compile-time error:\n" << infolog << endl;
        return false;
    }
    return true;
}
ShaderProgram::~ShaderProgram() { glDeleteProgram(m_ID); }

void ShaderProgram::use() const { glUseProgram(m_ID); };

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& ShaderProgram) {
    glDeleteProgram(m_ID);
    m_ID = ShaderProgram.m_ID;
    m_isCompiled = ShaderProgram.m_isCompiled;

    ShaderProgram.m_ID = 0;
    ShaderProgram.m_isCompiled = false;
    return *this;
}
ShaderProgram::ShaderProgram(ShaderProgram&& ShaderProgram) {
    m_ID = ShaderProgram.m_ID;
    m_isCompiled = ShaderProgram.m_isCompiled;

    ShaderProgram.m_ID = 0;
    ShaderProgram.m_isCompiled = false;
}
}  // namespace Renderer