#include "PieceManager.hpp"
#include <iostream>
#include "Shader.hpp"
#include "Texture.hpp"
#include "glm/ext/matrix_transform.hpp"

PieceManager::~PieceManager()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteBuffers(1, &m_instanceVBO);
    glDeleteVertexArrays(1, &m_vao);
}

PieceManager::PieceManager()
{
    init_pieces();
}

void PieceManager::init_pieces()
{
    m_modelMatrices.resize(16);
    // Pour l'instant on gère juste les 16 pions
    for (int i = 0; i < m_modelMatrices.size(); i++)
    {
        int       x        = i % 8;
        int       y        = i / 8 ? 6 : 1;
        glm::vec3 position = glm::vec3(x, 0.0f, y);
        m_modelMatrices[i] = glm::translate(glm::mat4(1.0f), position);
    }
}

void PieceManager::setup_buffers()
{
    // Lier et configurer les buffers pour les pions (VBO, EBO)
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_mesh.getVertices().size() * sizeof(glmax::Vertex), m_mesh.getVertices().data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_mesh.getIndices().size() * sizeof(uint32_t), m_mesh.getIndices().data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Lier les matrices d'instance aux attributs de vertex
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    // Attributs de position, normal et texture pour chaque pion
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, _position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, _normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, _tex_coord));

    // Lier et configurer le buffer pour les matrices d'instance
    glGenBuffers(1, &m_instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, m_modelMatrices.size() * sizeof(glm::mat4), m_modelMatrices.data(), GL_STATIC_DRAW);

    // Lier les matrices d'instance aux attributs de vertex
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (const GLvoid*)0);
    glVertexAttribDivisor(3, 1);
    //
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (const GLvoid*)(1 * sizeof(glm::vec4)));
    glVertexAttribDivisor(4, 1);
    //
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (const GLvoid*)(2 * sizeof(glm::vec4)));
    glVertexAttribDivisor(5, 1);
    //
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (const GLvoid*)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(6, 1);

    // Nettoyage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PieceManager::render(glmax::Shader& shader)
{
    glBindVertexArray(m_vao);

    // On boucle sur chaque sous-maille (submesh) pour dessiner ses instances
    for (const glmax::Submesh& submesh : m_mesh.getSubmeshes())
    {
        const glmax::Material& material = m_mesh.getMaterials().at(submesh.m_material_id);

        // Configurer les uniformes pour les propriétés du matériau
        shader.setUniform3fv("Kd", material.m_Kd);
        shader.setUniform3fv("Ka", material.m_Ka);
        shader.setUniform3fv("Ks", material.m_Ks);
        shader.setUniform1f("Ns", material.m_Ns);

        // Configurer la texture diffuse si elle existe
        shader.setUniform1i("map_Kd", material.m_mapKd.getID());
        material.m_mapKd.bind(material.m_mapKd.getID());
        // Ici, on utilise glDrawElementsInstanced pour dessiner toutes les instances
        glDrawElementsInstanced(GL_TRIANGLES, submesh.m_index_count, GL_UNSIGNED_INT, (const GLvoid*)(submesh.m_index_offset * sizeof(uint32_t)), m_modelMatrices.size());
        material.m_mapKd.unbind();
    }

    glBindVertexArray(0);
}

void PieceManager::setTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationX   = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));
    glm::mat4 rotationY   = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));
    glm::mat4 rotationZ   = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
    glm::mat4 scaling     = glm::scale(glm::mat4(1.0f), scale);
    m_modelMatrix         = translation * rotationX * rotationY * rotationZ * scaling;
}
