#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"

class Piece {
public:
    Piece() = default;
    ~Piece();
    void draw(const glmax::Shader& program);
    void setTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

    GLuint _vao;
    GLuint _vbo;
    GLuint _ebo;

    glmax::Mesh _mesh;
    void        setup_buffers();
private:
    glm::mat4   _model_matrix;
};