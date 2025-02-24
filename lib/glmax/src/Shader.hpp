#pragma once

#include <glad/glad.h>
#include <string>

namespace glmax {

class Shader {
public:
    Shader() = default;
    ~Shader();

    void   use() const;
    GLuint getID() const;
    void   loadShader(const std::string& vertexPath, const std::string& fragmentPath);

private:
    GLuint      programID;
    GLuint      compileShader(const std::string& path, GLenum type);
    std::string readFile(const std::string& path);
};

} // namespace glmax
