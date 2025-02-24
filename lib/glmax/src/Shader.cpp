#include "Shader.hpp"
#include <OpenGL/gl.h>
#include <fstream>
#include <iostream>
#include <sstream>

namespace glmax {

void Shader::loadShader(const std::string& vertexPath, const std::string& fragmentPath)
{
    GLuint vertexShader   = compileShader(vertexPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    GLint success{};
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(programID);
}

void Shader::use() const
{
    glUseProgram(programID);
}

GLuint Shader::getID() const
{
    return programID;
}

GLuint Shader::compileShader(const std::string& path, GLenum type)
{
    std::string sourceCode = readFile(path);
    const char* code       = sourceCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    return shader;
}

std::string Shader::readFile(const std::string& path)
{
    std::ifstream     file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

} // namespace glmax
