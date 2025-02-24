#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"

class Piece {
public:
    Piece() = default;
    ~Piece();
    void render(glmax::Shader& shader);
    void setTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    void setup_buffers();
    //
    glmax::Mesh m_mesh;

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    glm::mat4 m_modelMatrix;

    void      draw(const glmax::Submesh& submesh, const glmax::Material& material);
};