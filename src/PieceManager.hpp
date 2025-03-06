#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"
#include "buffers/EBO.hpp"
#include "buffers/VAO.hpp"
#include "buffers/VBO.hpp"
#include "utils.hpp"

struct PiecePositions {
    Type             piece_type;
    std::vector<int> black_position;
    std::vector<int> white_position;
};

class PieceManager {
public:
    explicit PieceManager(bool is_chessboard);

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
    std::vector<glm::mat4> m_modelMatrices;
    std::vector<glm::vec3> m_pieceColors;
    //
    // Colors
    glm::vec3 m_white_color = glm::vec3(0.961, 0.859, 0.635);
    glm::vec3 m_black_color = glm::vec3(0.1, 0.1, 0.1);

    //
    std::array<PiecePositions, 1> m_initial_positions = {
        PiecePositions{Type::PAWN, {8, 9, 10, 11, 12, 13, 14, 15}, {48, 49, 51, 50, 52, 53, 54, 55}},
        // PiecePositions{Type::ROOK, {0, 7}, {56, 63}},
        // PiecePositions{Type::KNIGHT, {1, 6}, {57, 62}},
        // PiecePositions{Type::BISHOP, {2, 5}, {58, 61}},
        // PiecePositions{Type::QUEEN, {3}, {59}},
        // PiecePositions{Type::KING, {4}, {60}}
    };
};