#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"
#include "buffers/VAO.hpp"
#include "buffers/VBO.hpp"
#include "buffers/EBO.hpp"
#include "utils.hpp"

class PieceManager {
public:
    explicit PieceManager(Type type);
    PieceManager(const PieceManager& other)            = delete;
    PieceManager& operator=(const PieceManager& other) = delete;
    ~PieceManager();
    void render(glmax::Shader& shader);
    void setTransform(unsigned int index, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    void setup_buffers();
    //
    glmax::Mesh m_mesh;

private:
    // TODO(smallboyc): Une classe par buffer
    VAO m_vao;
    VBO m_vbo;
    EBO m_ebo;
    //
    // Instancing
    VBO m_instanceVBO;
    std::vector<glm::mat4> m_modelMatrices;
};