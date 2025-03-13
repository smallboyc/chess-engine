#include "Renderer3D.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include "glm/ext/matrix_clip_space.hpp"

void Renderer3D::initCallbacks()
{
    window.key_callback = [&](int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_D && action == GLFW_PRESS)
        {
            isPieceDeleted = true;
        }
        if (!Camera.is_track_ball())
            Camera.process_input(key, action);
    };

    window.size_callback = [&](int width, int height) {
        window_width  = width;
        window_height = height;
    };

    window.cursor_position_callback = [&](double xpos, double ypos) {
        if (Camera.is_track_ball())
            Camera.process_mouse_movement(xpos, ypos);
    };

    window.scroll_callback = [&](double xoffset, double yoffset) {
        if (Camera.is_track_ball())
            Camera.process_scroll(yoffset);
    };

    window.mouse_button_callback = [](int button, int action, int mods) {
    };
}

void Renderer3D::run()
{
    window.run([&]() {
        glClearColor(0.847f, 0.82f, 0.929f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window_width) / static_cast<float>(window_height), 0.1f, 100.0f);
        shader.setUniformMatrix4fv("view", Camera.get_view_matrix());
        shader.setUniformMatrix4fv("projection", projection);

        shader.setUniform3fv("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
        shader.setUniform3fv("viewPos", Camera.get_position());
        shader.setUniform3fv("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

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
    });
}

void Renderer3D::update(float elapsedTime)
{
    // if (isAnimating)
    // {
    //     game_object_manager.movePiece(_chessboard.m_board, from, to, elapsedTime, animationStartTime, isAnimating);
    // }
}

void Renderer3D::render(float elapsedTime)
{
    shader.use();
    if (isPieceDeleted)
    {
        std::cout << "Piece deleted !"
                  << "\n";
        _chessboard.m_board[0].reset();
        game_object_manager.updatePiecesPositions(_chessboard.m_board);
        game_object_manager.updatePiecesData();
        isPieceDeleted = false;
    }
    game_object_manager.updatePiecesPositions(_chessboard.m_board);
    game_object_manager.renderGameObjects(shader);
}
