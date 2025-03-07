#include "PieceManager.hpp"
#include <iostream>
#include "Shader.hpp"
#include "Texture.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "utils.hpp"

PieceManager::~PieceManager()
{
}

PieceManager::PieceManager()
{
}

void PieceManager::init(std::array<std::unique_ptr<Piece>, 64>& chessboard)
{
    GameObject pawn;
    GameObject bishop;
    GameObject knight;
    GameObject rook;
    GameObject queen;
    GameObject king;
    for (int i = 0; i < chessboard.size(); i++)
    {
        if (chessboard[i])
        {
            if (chessboard[i]->get_type() == Type::Pawn)
            {
                glm::vec3 position = world_position(get_position(i));
                pawn.m_modelMatrices.push_back(glm::translate(glm::mat4(1.0f), position));
                pawn.m_pieceColors.push_back(chessboard[i]->getColor());
            }
            else if (chessboard[i]->get_type() == Type::Bishop)
            {
                glm::vec3 position = world_position(get_position(i));
                bishop.m_modelMatrices.push_back(glm::translate(glm::mat4(1.0f), position));
                bishop.m_pieceColors.push_back(chessboard[i]->getColor());
            }
            else if (chessboard[i]->get_type() == Type::Knight)
            {
                glm::vec3 position = world_position(get_position(i));
                knight.m_modelMatrices.push_back(glm::translate(glm::mat4(1.0f), position));
                knight.m_pieceColors.push_back(chessboard[i]->getColor());
            }
            else if (chessboard[i]->get_type() == Type::Rook)
            {
                glm::vec3 position = world_position(get_position(i));
                rook.m_modelMatrices.push_back(glm::translate(glm::mat4(1.0f), position));
                rook.m_pieceColors.push_back(chessboard[i]->getColor());
            }
            else if (chessboard[i]->get_type() == Type::Queen)
            {
                glm::vec3 position = world_position(get_position(i));
                queen.m_modelMatrices.push_back(glm::translate(glm::mat4(1.0f), position));
                queen.m_pieceColors.push_back(chessboard[i]->getColor());
            }
            else if (chessboard[i]->get_type() == Type::King)
            {
                glm::vec3 position = world_position(get_position(i));
                king.m_modelMatrices.push_back(glm::translate(glm::mat4(1.0f), position));
                king.m_pieceColors.push_back(chessboard[i]->getColor());
            }
        }
    }
    // std::cout << "Number of matrices: " << pawn.m_modelMatrices.size() << std::endl;
    // std::cout << "Number of colors: " << pawn.m_pieceColors.size() << std::endl;

    m_gameObjects.insert(std::make_pair(Type::Pawn, std::move(pawn)));
    m_gameObjects.insert(std::make_pair(Type::Bishop, std::move(bishop)));
    m_gameObjects.insert(std::make_pair(Type::Rook, std::move(rook)));
    m_gameObjects.insert(std::make_pair(Type::Knight, std::move(knight)));
    m_gameObjects.insert(std::make_pair(Type::Queen, std::move(queen)));
    m_gameObjects.insert(std::make_pair(Type::King, std::move(king)));
}

void PieceManager::loadAllMesh()
{
    for (auto& [type, gameObject] : m_gameObjects)
    {
        if (type == Type::Pawn)
            gameObject.loadMesh("pawn/pawn.obj", "pawn");
        if (type == Type::Bishop)
            gameObject.loadMesh("bishop/bishop.obj", "bishop");
        if (type == Type::Rook)
            gameObject.loadMesh("rook/rook.obj", "rook");
        if (type == Type::Knight)
            gameObject.loadMesh("knight/knight.obj", "knight");
        if (type == Type::Queen)
            gameObject.loadMesh("queen/queen.obj", "queen");
        if (type == Type::King)
            gameObject.loadMesh("king/king.obj", "king");
        gameObject.setupBuffers();
    }
}

void GameObject::loadMesh(const std::string& path, const std::string& name)
{
    m_mesh.load(path, name);
}

void GameObject::setupBuffers()
{
    // Lier et configurer les buffers pour les pions (VBO, EBO)
    m_vbo.init();
    m_vbo.bind();
    m_vbo.setData(m_mesh.getVertices().data(), m_mesh.getVertices().size() * sizeof(glmax::Vertex));
    m_vbo.unbind();

    m_ebo.init();
    m_ebo.bind();
    m_ebo.setData(m_mesh.getIndices().data(), m_mesh.getIndices().size() * sizeof(uint32_t));
    m_ebo.unbind();

    // Lier les matrices d'instance aux attributs de vertex
    m_vao.init();
    m_vao.bind();
    m_vbo.bind();
    m_ebo.bind();

    // Attributs de position, normal et texture pour chaque pion
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, _position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, _normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glmax::Vertex), (const GLvoid*)offsetof(glmax::Vertex, _tex_coord));

    // Lier et configurer le buffer pour les matrices d'instance
    if (!m_modelMatrices.empty())
    {
        m_instanceVBO.init();
        m_instanceVBO.bind();
        m_instanceVBO.setData(m_modelMatrices.data(), m_modelMatrices.size() * sizeof(glm::mat4));

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
        m_instanceVBO.unbind();
    }
    if (!m_pieceColors.empty())
    {
        m_colorVBO.init();
        m_colorVBO.bind();
        m_colorVBO.setData(m_pieceColors.data(), m_pieceColors.size() * sizeof(glm::vec3));
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const GLvoid*)0);
        glVertexAttribDivisor(7, 1);
        m_colorVBO.unbind();
    }
    m_vao.unbind();
}

void GameObject::render(glmax::Shader& shader)
{
    m_vao.bind();
    // On boucle sur chaque sous-maille (submesh) pour dessiner ses instances
    for (const glmax::Submesh& submesh : m_mesh.getSubmeshes())
    {
        const glmax::Material& material = m_mesh.getMaterials().at(submesh.m_material_id);

        // Configurer les uniformes pour les propriétés du matériau
        // shader.setUniform3fv("Kd", glm::vec3(0.961, 0.859, 0.635));
        shader.setUniform3fv("Ka", material.m_Ka);
        shader.setUniform3fv("Ks", material.m_Ks);
        shader.setUniform1f("Ns", material.m_Ns);

        if (material.m_hasMapKd)
        {
            shader.setUniform1i("map_Kd", material.m_mapKd.getID());
            material.m_mapKd.bind(material.m_mapKd.getID());
            shader.setUniform1i("useTexture", true);
        }
        else
        {
            shader.setUniform1i("useTexture", false);
        }
        // Ici, on utilise glDrawElementsInstanced pour dessiner toutes les instances
        if (m_modelMatrices.size() > 1)
            glDrawElementsInstanced(GL_TRIANGLES, submesh.m_index_count, GL_UNSIGNED_INT, (const GLvoid*)(submesh.m_index_offset * sizeof(uint32_t)), m_modelMatrices.size());
        else
            glDrawElements(GL_TRIANGLES, submesh.m_index_count, GL_UNSIGNED_INT, (const GLvoid*)(submesh.m_index_offset * sizeof(uint32_t)));
        //
        if (material.m_hasMapKd)
            material.m_mapKd.unbind();
    }

    m_vao.unbind();
}

void GameObject::setTransform(const unsigned int index, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
    glm::mat4 translation  = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationX    = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));
    glm::mat4 rotationY    = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));
    glm::mat4 rotationZ    = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
    glm::mat4 scaling      = glm::scale(glm::mat4(1.0f), scale);
    m_modelMatrices[index] = translation * rotationX * rotationY * rotationZ * scaling;
    m_instanceVBO.bind();
    m_instanceVBO.setData(m_modelMatrices.data(), m_modelMatrices.size() * sizeof(glm::mat4));
    m_instanceVBO.unbind();
}
