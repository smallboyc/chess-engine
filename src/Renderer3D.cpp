#include "Renderer3D.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include "glm/ext/matrix_clip_space.hpp"

void Renderer3D::window_size_callback(int width, int height)
{
    window_width  = width;
    window_height = height;
}

void Renderer3D::delete_piece_callback(int key, int action)
{
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        isPieceDeleted = true;
    }
}

void Renderer3D::init()
{
    // init shader
    m_shader.loadShader("model.vs.glsl", "model.fs.glsl");
    // init all pieces
    m_gameObjectManager.updatePiecesPositions(_chessboard.m_board);
    m_gameObjectManager.loadAllPieces();
    // init chessboard
    m_gameObjectManager.loadChessboard();
}

void Renderer3D::run()
{
    glClearColor(0.847f, 0.82f, 0.929f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window_width) / static_cast<float>(window_height), 0.1f, 100.0f);
    m_shader.setUniformMatrix4fv("view", m_camera.get_view_matrix());
    m_shader.setUniformMatrix4fv("projection", projection);

    m_shader.setUniform3fv("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
    m_shader.setUniform3fv("viewPos", m_camera.get_position());
    m_shader.setUniform3fv("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    auto                         current_time = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed      = current_time - start_time;
    float                        elapsed_time = elapsed.count();

    if (elapsed_time > 6.0f && !isAnimating)
    {
        isAnimating        = true;
        animationStartTime = elapsed_time;
    }
    // UPDATE
    update(elapsed_time);
    // RENDER
    render(elapsed_time);

    ImGui::Begin("Test");
    ImGui::Text("Hello Test");
    ImGui::End();
}

void Renderer3D::update(float elapsedTime)
{
    // if (isAnimating)
    // {
    //     m_gameObjectManager.movePiece(_chessboard.m_board, from, to, elapsedTime, animationStartTime, isAnimating);
    // }
}

void Renderer3D::render(float elapsedTime)
{
    m_shader.use();
    if (isPieceDeleted)
    {
        std::cout << "Piece deleted !"
                  << "\n";
        _chessboard.m_board[0].reset();
        m_gameObjectManager.updatePiecesPositions(_chessboard.m_board);
        m_gameObjectManager.updatePiecesData();
        isPieceDeleted = false;
    }
    m_gameObjectManager.updatePiecesPositions(_chessboard.m_board);
    m_gameObjectManager.renderGameObjects(m_shader);
}
