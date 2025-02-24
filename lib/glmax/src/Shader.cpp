#include "Shader.hpp"
#include <OpenGL/gl.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"

namespace glmax {

void Shader::loadShader(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string relativePath   = "../../shaders/";
    GLuint      vertexShader   = compileShader(relativePath + vertexPath, GL_VERTEX_SHADER);
    GLuint      fragmentShader = compileShader(relativePath + fragmentPath, GL_FRAGMENT_SHADER);
    m_programID                = glCreateProgram();
    glAttachShader(m_programID, vertexShader);
    glAttachShader(m_programID, fragmentShader);
    glLinkProgram(m_programID);

    GLint success{};
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_programID);
}

void Shader::use() const
{
    glUseProgram(m_programID);
}

GLuint Shader::getID() const
{
    return m_programID;
}

GLuint Shader::compileShader(const std::string& path, GLenum type)
{
    std::string sourceCode = readFile(path);
    const char* code       = sourceCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    GLint success{};
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

// - Uniforms
void Shader::setUniformMatrix4fv(const std::string& name, const glm::mat4& value)
{
    std::optional<GLint> location = getUniformLocation(name);
    if (location.has_value())
        glUniformMatrix4fv(location.value(), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform3fv(const std::string& name, const glm::vec3& value)
{
    std::optional<GLint> location = getUniformLocation(name);
    if (location.has_value())
        glUniform3fv(location.value(), 1, glm::value_ptr(value));
}

void Shader::setUniform1f(const std::string& name, const float value)
{
    std::optional<GLint> location = getUniformLocation(name);
    if (location.has_value())
        glUniform1f(location.value(), value);
}

void Shader::setUniform1i(const std::string& name, const int value)
{
    std::optional<GLint> location = getUniformLocation(name);
    if (location.has_value())
        glUniform1i(location.value(), value);
}

// - Uniform location
std::optional<GLint> Shader::getUniformLocation(const std::string& name)
{
    if (m_uniformCache.find(name) != m_uniformCache.end())
    {
        return m_uniformCache[name];
    }
    GLint location = glGetUniformLocation(m_programID, name.c_str());
    if (location == -1)
    {
        // std::cerr << "Uniform " << name << " not found!" << std::endl;
        return std::nullopt;
    }
    m_uniformCache[name] = location;
    return location;
}

} // namespace glmax
