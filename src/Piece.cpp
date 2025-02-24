#include "Piece.hpp"
#include "Shader.hpp"
#include "glm/gtc/type_ptr.hpp"

Piece::~Piece()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);
}

void Piece::draw(const glmax::Shader& program)
{
    // Model
    GLint modelLoc = glGetUniformLocation(program.getID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(_model_matrix));

    // Materials
    GLint kdLoc = glGetUniformLocation(program.getID(), "Kd");
    GLint kaLoc = glGetUniformLocation(program.getID(), "Ka");
    GLint ksLoc = glGetUniformLocation(program.getID(), "Ks");
    GLint nsLoc = glGetUniformLocation(program.getID(), "Ns");

    glBindVertexArray(_vao);
    for (glmax::Submesh& submesh : _mesh.getSubmeshes())
    {
        const glmax::Material& material = _mesh.getMaterials().at(submesh.m_material_id);
        //
        glUniform3f(kdLoc, material.m_Kd.x, material.m_Kd.y, material.m_Kd.z);
        glUniform3f(kaLoc, material.m_Ka.x, material.m_Ka.y, material.m_Ka.z);
        glUniform3f(ksLoc, material.m_Ks.x, material.m_Ks.y, material.m_Ks.z);
        glUniform1f(nsLoc, material.m_Ns);
        // Textures
        GLint mapKdLoc = glGetUniformLocation(program.getID(), "map_Kd");
        glUniform1i(mapKdLoc, material.m_mapKd.getID());
        material.m_mapKd.bind(material.m_mapKd.getID());

        glDrawElements(GL_TRIANGLES, submesh.m_index_count, GL_UNSIGNED_INT, (void*)(submesh.m_index_offset * sizeof(unsigned int)));

        material.m_mapKd.unbind();
    }
    glBindVertexArray(0);
}

void Piece::setTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationX   = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));
    glm::mat4 rotationY   = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));
    glm::mat4 rotationZ   = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
    glm::mat4 scaling     = glm::scale(glm::mat4(1.0f), scale);
    _model_matrix         = translation * rotationX * rotationY * rotationZ * scaling;
}

void Piece::setup_buffers()
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _mesh.getVertices().size() * sizeof(glmax::Vertex), _mesh.getVertices().data(), GL_STATIC_DRAW);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh.getIndices().size() * sizeof(uint32_t), _mesh.getIndices().data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, _position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, _normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, _tex_coord));

    glBindVertexArray(0);
}
