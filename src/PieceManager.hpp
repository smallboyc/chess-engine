#pragma once
#include "Mesh.hpp"
#include "Piece.hpp"
#include "Shader.hpp"
#include "buffers/EBO.hpp"
#include "buffers/VAO.hpp"
#include "buffers/VBO.hpp"

class PieceManager {
public:
    explicit PieceManager(std::array<std::unique_ptr<Piece>, 64>& chessboard);

    PieceManager(const PieceManager& other)            = delete;
    PieceManager& operator=(const PieceManager& other) = delete;
    ~PieceManager();
    void render(glmax::Shader& shader);
    void setTransform(unsigned int index, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    void setupBuffers();
    void loadMesh(const std::string& path, const std::string& name);
    //

private:
    glmax::Mesh m_mesh;
    //
    // Buffers
    VAO m_vao;
    VBO m_vbo;
    EBO m_ebo;
    //
    // Instancing
    VBO                    m_instanceVBO;
    VBO                    m_colorVBO;
    std::vector<glm::mat4> m_modelMatrices;
    std::vector<glm::vec3> m_pieceColors;
};