#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"
#include "buffers/EBO.hpp"
#include "buffers/VAO.hpp"
#include "buffers/VBO.hpp"

// A GameObject is a game object (chessboard, pawn, queen) that contains information about all its instances : a single mesh, vao,vbo,ebo is used.
class GameObject {
public:
    void render(glmax::Shader& shader) const;
    void setTransform(unsigned int index, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    void loadMesh(const std::string& path, const std::string& name);
    void setupBuffers();
    // have better control over instance buffers
    void                                           updateMatInstancingBuffer();
    void                                           updateColorInstancingBuffer();
    void                                           clearInstancingBuffers();
    void                                           pushMatrix(const glm::vec3& position);
    void                                           pushColor(const glm::vec3& color);
    void                                           updateBoardRelations(int i);
    std::unordered_map<unsigned int, unsigned int> m_board_instance_relation; // board index / model matrix index in m_modelMatrices

private:
    // single mesh
    glmax::Mesh m_mesh;
    //
    // Buffers
    VAO m_vao;
    VBO m_vbo;
    EBO m_ebo;
    //
    // Instancing
    VBO m_instanceVBO;
    VBO m_colorVBO;
    // Dis quels indices correspond à la matrice stocké sur le board
    std::vector<glm::mat4> m_modelMatrices;
    std::vector<glm::vec3> m_pieceColors;
};
