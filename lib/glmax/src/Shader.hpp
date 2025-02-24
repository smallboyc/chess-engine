#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include "glm/fwd.hpp"

namespace glmax {

class Shader {
public:
    Shader() = default;
    ~Shader();

    void   use() const;
    GLuint getID() const;
    void   loadShader(const std::string& vertexPath, const std::string& fragmentPath);
    void   setUniformMatrix4fv(const std::string& name, const glm::mat4& value);
    void   setUniform3fv(const std::string& name, const glm::vec3& value);
    void   setUniform1f(const std::string& name, float value);
    void   setUniform1i(const std::string& name, int value);

private:
    GLuint                                 m_programID;
    std::unordered_map<std::string, GLint> m_uniformCache;
    //
    GLuint               compileShader(const std::string& path, GLenum type);
    std::string          readFile(const std::string& path);
    std::optional<GLint> getUniformLocation(const std::string& name);
};

} // namespace glmax
