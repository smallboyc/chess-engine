#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"
#include "buffers/VAO.hpp"
#include "buffers/VBO.hpp"
#include "buffers/EBO.hpp"

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
    VAO m_vao;
    VBO m_vbo;
    EBO m_ebo;
    //
    glm::mat4 m_modelMatrix;
    // Instancing
    VBO m_instanceVBO;
    std::vector<glm::mat4> m_modelMatrices;
};