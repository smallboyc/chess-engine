#include "Renderer3D.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include "Animation.hpp"
#include "glm/ext/matrix_clip_space.hpp"

void Renderer3D::window_size_callback(int width, int height)
{
    window_width  = width;
    window_height = height;
}

void Renderer3D::toggle_active_camera_callback(int key, int action)
{
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        m_camera.toggle_lock();
    }
}

void Renderer3D::init(std::array<std::unique_ptr<Piece>, 64>& chessboard)
{
    // init basic shader
    m_shader.loadShader("model.vs.glsl", "model.fs.glsl");
    // init skybox shader
    m_skybox_shader.loadShader("skybox.vs.glsl", "skybox.fs.glsl");
    m_skybox.loadCubeMaps();
    m_skybox.setupBuffers();
    // init all pieces
    m_gameObjectManager.updatePiecesPositions(chessboard);
    m_gameObjectManager.loadAllPieces();
    // init chessboard
    m_gameObjectManager.loadChessboard();
}

void Renderer3D::run(std::array<std::unique_ptr<Piece>, 64>& chessboard, std::optional<MoveProcessing>& move_processing, Animation& animation)
{
    glClearColor(0.847f, 0.82f, 0.929f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window_width) / static_cast<float>(window_height), 0.1f, 100.0f);

    // RENDER SKYBOX (start)
    glDepthFunc(GL_LEQUAL); // Pour éviter les problèmes de profondeur
    glDepthMask(GL_FALSE);

    m_skybox_shader.use();
    glm::mat4 view = glm::mat4(glm::mat3(m_camera.get_view_matrix())); // Supprimer la translation

    m_skybox_shader.setUniform1i("skybox", 0);
    m_skybox_shader.setUniformMatrix4fv("view", view);
    m_skybox_shader.setUniformMatrix4fv("projection", projection);

    m_skybox.m_vao.bind();
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox.getTextureID());
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_skybox.m_vao.unbind();

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    // RENDER SKYBOX (end)

    //
    m_shader.use();
    m_shader.setUniformMatrix4fv("view", m_camera.get_view_matrix());
    m_shader.setUniformMatrix4fv("projection", projection);

    m_shader.setUniform3fv("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
    m_shader.setUniform3fv("viewPos", m_camera.get_position());
    m_shader.setUniform3fv("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    //

    auto                         current_time = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed      = current_time - start_time;
    float                        elapsed_time = elapsed.count();
    //
    // UPDATE
    if (move_processing.has_value())
    {
        if (!animation.isAnimating)
        {
            animation.isAnimating        = true;
            animation.animationStartTime = elapsed_time;
        }

        // -> ANIMATION
        m_gameObjectManager.movePiece(chessboard, move_processing.value(), elapsed_time, animation);

        // -> Update data after the animation's end
        if (!animation.isAnimating)
        {
            std::cout << "Update data after the end of animation!"
                      << "\n";
            m_gameObjectManager.updatePiecesPositions(chessboard);
            m_gameObjectManager.updatePiecesData();
            move_processing.reset(); // reset the move processing after animation (from / to reinitialized)
        }
    }
    // RENDER OBJECTS (PIECE, CHESSBOARD)
    m_gameObjectManager.renderGameObjects(m_shader);
}
