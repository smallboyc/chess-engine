#include "Skybox.hpp"
#include <iostream>
#include "glm/fwd.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Skybox::setup_buffers()
{
    // Initialiser et configurer les buffers pour les pions (VBO, EBO)
    m_vbo.init();
    m_vbo.bind();
    m_vbo.set_data(m_cube_coordinates.data(), m_cube_coordinates.size() * sizeof(glm::vec3));
    m_vbo.unbind();

    m_ebo.init();
    m_ebo.bind();
    m_ebo.set_data(m_cube_indices.data(), m_cube_indices.size() * sizeof(unsigned int));
    m_ebo.unbind();

    // Initialiser et configurer le VAO
    m_vao.init();
    m_vao.bind();
    m_vbo.bind();
    m_ebo.bind();

    // Attributs de position pour chaque sommet
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    m_vao.unbind();
}

void Skybox::load_cube_maps()
{
    // Générer une texture pour la cubemap
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
    // Paramètres de la texture
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Charger chaque face de la cubemap
    for (unsigned int i = 0; i < m_cube_faces.size(); i++)
    {
        int            width      = 0;
        int            height     = 0;
        int            nrChannels = 0;
        unsigned char* data       = stbi_load(m_cube_faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);

            // Assigner la texture à la face correspondante de la cubemap
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "Cubemap texture failed to load at path: " << m_cube_faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    // Désactiver la texture cubemap
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
