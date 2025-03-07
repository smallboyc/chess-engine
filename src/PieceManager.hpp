#pragma once
#include "Mesh.hpp"
#include "Piece.hpp"
#include "Shader.hpp"
#include "buffers/EBO.hpp"
#include "buffers/VAO.hpp"
#include "buffers/VBO.hpp"

class GameObject {
public:
    void        render(glmax::Shader& shader);
    void        setTransform(unsigned int index, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    void        setupBuffers();
    void        loadMesh(const std::string& path, const std::string& name);
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

class PieceManager {
public:
    PieceManager();

    PieceManager(const PieceManager& other)            = delete;
    PieceManager& operator=(const PieceManager& other) = delete;
    ~PieceManager();
    void init(std::array<std::unique_ptr<Piece>, 64>& chessboard);
    void loadAllMesh();
    std::unordered_map<Type, GameObject> m_gameObjects;
    GameObject m_chessboard;
};