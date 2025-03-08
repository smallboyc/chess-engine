#include "Renderer3D.hpp"
#include "glm/ext/matrix_clip_space.hpp"

void Renderer3D::initCallbacks()
{
    window.key_callback = [&](int key, int scancode, int action, int mods) {
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
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1280) / static_cast<float>(720), 0.1f, 100.0f);
        shader.setUniformMatrix4fv("view", Camera.get_view_matrix());
        shader.setUniformMatrix4fv("projection", projection);

        shader.setUniform3fv("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
        shader.setUniform3fv("viewPos", Camera.get_position());
        shader.setUniform3fv("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        auto                         current_time = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed      = current_time - start_time;
        float                        elapsed_time = elapsed.count();

        if (elapsed_time > 12.0f && !isAnimating && isFirstTime)
        {
            isAnimating        = true;
            animationStartTime = elapsed_time;
            isFirstTime        = false;
        }

        updatePiecePosition(elapsed_time);

        shader.use();
        piece_manager.m_chessboard.render(shader);

        for (auto& [type, gameObject] : piece_manager.m_gameObjects)
        {
            gameObject.render(shader);
        }

        ImGui::Begin("Test");
        ImGui::Text("Hello Test");
        ImGui::End();
    });
}

void Renderer3D::updatePiecePosition(float elapsedTime)
{
    if (isAnimating)
    {
        float t = (elapsedTime - animationStartTime) / animationDuration;
        if (t >= 1.0f)
        {
            t           = 1.0f;
            isAnimating = false;
        }

        glm::vec3 startPos   = world_position(get_position(from));
        glm::vec3 endPos     = world_position(get_position(to));
        glm::vec3 currentPos = glm::mix(startPos, endPos, t);

        for (auto& [type, gameObject] : piece_manager.m_gameObjects)
        {
            for (auto& [index, instance_index] : gameObject.m_board_instance_relation)
            {
                if (index == from)
                {
                    gameObject.setTransform(instance_index, currentPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
                    if (!isAnimating)
                    {
                        gameObject.m_board_instance_relation[to] = instance_index;
                        gameObject.m_board_instance_relation.erase(from);
                    }
                }
            }
        }
    }
}