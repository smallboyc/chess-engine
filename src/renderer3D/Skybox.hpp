#pragma once

#include <array>
#include <glm/glm.hpp>
#include <string>
#include "buffers/VAO.hpp"
#include "renderer3D/buffers/EBO.hpp"
#include "renderer3D/buffers/VBO.hpp"

class Skybox {
public:
    Skybox() = default;
    void         setupBuffers();
    void         loadCubeMaps();
    unsigned int getTextureID() const { return m_textureID; };
    VAO          m_vao;

private:
    std::array<glm::vec3, 8> m_cube_coordinates = {
        glm::vec3{-1.0f, -1.0f, 1.0f},  // 0
        glm::vec3{1.0f, -1.0f, 1.0f},   // 1
        glm::vec3{1.0f, -1.0f, -1.0f},  // 2
        glm::vec3{-1.0f, -1.0f, -1.0f}, // 3
        glm::vec3{-1.0f, 1.0f, 1.0f},   // 4
        glm::vec3{1.0f, 1.0f, 1.0f},    // 5
        glm::vec3{1.0f, 1.0f, -1.0f},   // 6
        glm::vec3{-1.0f, 1.0f, -1.0f}   // 7
    };
    std::array<unsigned int, 36> m_cube_indices = {
        // Face avant
        0, 1, 4,
        1, 5, 4,
        // Face droite
        1, 2, 5,
        2, 6, 5,
        // Face arrière
        2, 3, 6,
        3, 7, 6,
        // Face gauche
        3, 0, 7,
        0, 4, 7,
        // Face du dessus
        4, 5, 7,
        5, 6, 7,
        // Face du dessous
        0, 1, 3,
        1, 2, 3
    };
    std::array<std::string, 6> m_cube_faces = {
        "../../assets/images/skybox/right.jpg",
        "../../assets/images/skybox/left.jpg",
        "../../assets/images/skybox/top.jpg",
        "../../assets/images/skybox/bottom.jpg",
        "../../assets/images/skybox/front.jpg",
        "../../assets/images/skybox/back.jpg"
    };

    unsigned int m_textureID;

    VBO m_vbo;
    EBO m_ebo;
};