#pragma once
#include "Mesh.hpp"
#include "Shader.hpp"
#include "buffers/EBO.hpp"
#include "buffers/VAO.hpp"
#include "buffers/VBO.hpp"

class GameObject {
public:
    void        render(glmax::Shader& shader) const;
    void        setTransform(unsigned int index, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    void        loadMesh(const std::string& path, const std::string& name);
    void        setupBuffers();
    glmax::Mesh m_mesh;
    //
    // Buffers
    VAO m_vao;
    VBO m_vbo;
    EBO m_ebo;
    //
    // Instancing
    VBO                          m_instanceVBO;
    VBO                          m_colorVBO;
    std::unordered_map<unsigned int, unsigned int> m_board_instance_relation;
    std::vector<glm::mat4>       m_modelMatrices;
    std::vector<glm::vec3>       m_pieceColors;
};