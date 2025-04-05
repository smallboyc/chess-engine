#include "GameObject.hpp"
#include "Texture.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "renderer3D/utils.hpp"

void GameObject::render(glmax::Shader& shader, Settings& settings) const
{
    m_vao.bind();
    // We loop over each submesh to draw its instances
    for (const glmax::Submesh& submesh : m_mesh.get_submeshes())
    {
        // Get the material for the current submesh
        const glmax::Material& material = m_mesh.get_materials().at(submesh.m_material_id);

        shader.set_uniform_3fv("Ka", material.m_Ka);
        shader.set_uniform_3fv("Ks", material.m_Ks);
        shader.set_uniform_1f("Ns", material.m_Ns);

        // Check if the material has a texture
        if (material.m_hasMapKd)
        {
            glm::vec3 custom_cell_color = Renderer3DUtils::imgui_vec4_to_glm_vec3(settings.get_secondary_color());
            if (material.m_name == "BLACK_CELL")
            {
                custom_cell_color = Renderer3DUtils::imgui_vec4_to_glm_vec3(settings.get_primary_color());
            }
            shader.set_uniform_3fv("colorFactor", custom_cell_color);
            shader.set_uniform_1i("map_Kd", material.m_mapKd.getID());
            material.m_mapKd.bind(material.m_mapKd.getID());
            shader.set_uniform_1i("useTexture", true);
        }
        else
        {
            shader.set_uniform_3fv("colorFactor", glm::vec3(1.0f, 1.0f, 1.0f));
            shader.set_uniform_1i("useTexture", false);
        }

        // Draw with instancing if we have more than one model matrix in the vector
        if (m_model_matrices.size() > 1)
            glDrawElementsInstanced(GL_TRIANGLES, submesh.m_index_count, GL_UNSIGNED_INT, (const GLvoid*)(submesh.m_index_offset * sizeof(uint32_t)), m_model_matrices.size());
        else if (m_model_matrices.size() != 0) // Single instance = chessboard
            glDrawElements(GL_TRIANGLES, submesh.m_index_count, GL_UNSIGNED_INT, (const GLvoid*)(submesh.m_index_offset * sizeof(uint32_t)));
        if (material.m_hasMapKd)
            material.m_mapKd.unbind();
    }

    m_vao.unbind();
}

// Apply all transformations to the model matrix
void GameObject::set_transform(const unsigned int index, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    glm::mat4 translation   = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationX     = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));
    glm::mat4 rotationY     = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));
    glm::mat4 rotationZ     = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
    glm::mat4 scaling       = glm::scale(glm::mat4(1.0f), scale);
    m_model_matrices[index] = translation * rotationX * rotationY * rotationZ * scaling;
    //
    update_mat_instancing_buffer();
}

// Fill all buffers thanks to the mesh loaded
void GameObject::setup_buffers()
{
    // VBO
    m_vbo.init();
    m_vbo.bind();
    m_vbo.set_data(m_mesh.get_vertices().data(), m_mesh.get_vertices().size() * sizeof(glmax::Vertex));
    m_vbo.unbind();

    // EBO
    m_ebo.init();
    m_ebo.bind();
    m_ebo.set_data(m_mesh.get_indices().data(), m_mesh.get_indices().size() * sizeof(uint32_t));
    m_ebo.unbind();

    // VAO
    m_vao.init();
    m_vao.bind();
    m_vbo.bind();
    m_ebo.bind();

    // Data from CPU to GPU (shaders)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, m_position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, m_normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, m_tex_coord));

    // Instancing
    if (!m_model_matrices.empty())
    {
        m_instanceVBO.init();
        m_instanceVBO.bind();
        m_instanceVBO.set_data(m_model_matrices.data(), m_model_matrices.size() * sizeof(glm::mat4));

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

        m_instanceVBO.unbind();
    }
    if (!m_piece_colors.empty())
    {
        m_colorVBO.init();
        m_colorVBO.bind();
        m_colorVBO.set_data(m_piece_colors.data(), m_piece_colors.size() * sizeof(glm::vec3));
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const GLvoid*)0);
        glVertexAttribDivisor(7, 1);
        m_colorVBO.unbind();
    }
    // End of Instancing
    
    m_vao.unbind();
}

void GameObject::update_mat_instancing_buffer()
{
    m_vao.bind();
    //
    m_instanceVBO.bind();
    m_instanceVBO.set_data(m_model_matrices.data(), m_model_matrices.size() * sizeof(glm::mat4));
    m_instanceVBO.unbind();
    //
    m_vao.unbind();
}

void GameObject::update_color_instancing_buffer()
{
    m_vao.bind();
    //
    m_colorVBO.bind();
    m_colorVBO.set_data(m_piece_colors.data(), m_piece_colors.size() * sizeof(glm::vec3));
    m_colorVBO.unbind();
    //
    m_vao.unbind();
}

void GameObject::update_board_relations(int i)
{
    m_board_instance_relation[i] = m_model_matrices.size() - 1;
}

void GameObject::load_mesh(const std::string& path, const std::string& name)
{
    m_mesh.load(path, name);
}

void GameObject::clear_instancing_buffers()
{
    m_model_matrices.clear();
    m_piece_colors.clear();
    m_board_instance_relation.clear();
}

void GameObject::push_matrix(const glm::vec3& position)
{
    m_model_matrices.push_back(glm::translate(glm::mat4(1.0f), position));
}

void GameObject::push_color(const glm::vec3& color)
{
    m_piece_colors.push_back(color);
}