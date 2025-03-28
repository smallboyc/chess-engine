#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"
#include "buffers/EBO.hpp"
#include "buffers/VAO.hpp"
#include "buffers/VBO.hpp"
#include "game2D/gui/Settings.hpp"

// A GameObject is a game object (chessboard, pawn, queen) that contains information about all its instances : a single mesh, vao,vbo,ebo is used.
class GameObject {
public:
    void render(glmax::Shader& shader, Settings& settings) const;
    void set_transform(unsigned int index, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    void load_mesh(const std::string& path, const std::string& name);
    void setup_buffers();
    // have better control over instance buffers
    void update_mat_instancing_buffer();
    void update_color_instancing_buffer();
    void clear_instancing_buffers();
    void push_matrix(const glm::vec3& position);
    void push_color(const glm::vec3& color);
    void update_board_relations(int i);
    //
    const std::unordered_map<unsigned int, unsigned int>& get_board_instance_relation() { return m_board_instance_relation; }

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
    std::vector<glm::mat4>                         m_model_matrices;
    std::vector<glm::vec3>                         m_piece_colors;
    std::unordered_map<unsigned int, unsigned int> m_board_instance_relation; // board index / model matrix index in m_modelMatrices
};
