#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"

class PieceManager {
public:
    PieceManager();
    PieceManager(const PieceManager& other)            = delete;
    PieceManager& operator=(const PieceManager& other) = delete;
    ~PieceManager();
    void render(glmax::Shader& shader);
    void setTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    void setup_buffers();
    //
    void        init_pieces();
    glmax::Mesh m_mesh;

private:
    // TODO(smallboyc): Une classe par buffer
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    //
    glm::mat4 m_modelMatrix;
    // Instancing
    GLuint                 m_instanceVBO;
    std::vector<glm::mat4> m_modelMatrices;
};