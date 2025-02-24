#include "Piece.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "glm/ext/matrix_transform.hpp"

Piece::~Piece()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

void Piece::render(glmax::Shader& shader)
{
    shader.setUniformMatrix4fv("model", m_modelMatrix);

    glBindVertexArray(m_vao);
    for (const glmax::Submesh& submesh : m_mesh.getSubmeshes())
    {
        const glmax::Material& material = m_mesh.getMaterials().at(submesh.m_material_id);

        shader.setUniform3fv("Kd", material.m_Kd);
        shader.setUniform3fv("Ka", material.m_Ka);
        shader.setUniform3fv("Ks", material.m_Ks);
        shader.setUniform1f("Ns", material.m_Ns);

        // Textures
        shader.setUniform1i("map_Kd", material.m_mapKd.getID());

        draw(submesh, material);
    }
    glBindVertexArray(0);
}

void Piece::draw(const glmax::Submesh& submesh, const glmax::Material& material)
{
    material.m_mapKd.bind(material.m_mapKd.getID());
    glDrawElements(GL_TRIANGLES, submesh.m_index_count, GL_UNSIGNED_INT, (void*)(submesh.m_index_offset * sizeof(unsigned int)));
    material.m_mapKd.unbind();
}

void Piece::setTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationX   = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));
    glm::mat4 rotationY   = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));
    glm::mat4 rotationZ   = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
    glm::mat4 scaling     = glm::scale(glm::mat4(1.0f), scale);
    m_modelMatrix         = translation * rotationX * rotationY * rotationZ * scaling;
}

void Piece::setup_buffers()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_mesh.getVertices().size() * sizeof(glmax::Vertex), m_mesh.getVertices().data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_mesh.getIndices().size() * sizeof(uint32_t), m_mesh.getIndices().data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, _position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, _normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, _tex_coord));

    glBindVertexArray(0);
}
