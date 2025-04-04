#include "Renderer3D.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include "Animation.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "utils.hpp"

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
    m_basic_shader.load_shader("model.vs.glsl", "model.fs.glsl");
    // init skybox shader
    m_skybox_shader.load_shader("skybox.vs.glsl", "skybox.fs.glsl");
    m_skybox.load_cube_maps();
    m_skybox.setup_buffers();
    // init all pieces
    m_game_object_manager.update_pieces_positions(chessboard);
    m_game_object_manager.load_all_pieces();
    // init chessboard
    m_game_object_manager.load_chessboard();
}

void Renderer3D::run(std::array<std::unique_ptr<Piece>, 64>& chessboard, std::optional<MoveProcessing>& move_processing, Animation& animation, Settings& settings)
{
    glClearColor(0.847f, 0.82f, 0.929f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // TIME
    auto                         current_time = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed      = current_time - start_time;
    float                        elapsed_time = elapsed.count();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window_width) / static_cast<float>(window_height), 0.1f, 100.0f);

    // RENDER SKYBOX (start)
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    m_skybox_shader.use();
    glm::mat4 view = glm::mat4(glm::mat3(m_camera.get_view_matrix()));

    m_skybox_shader.set_uniform_1i("skybox", 0);
    m_skybox_shader.set_uniform_matrix_4fv("view", view);
    m_skybox_shader.set_uniform_matrix_4fv("projection", projection);

    m_skybox.m_vao.bind();
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox.get_textureID());
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_skybox.m_vao.unbind();

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    // RENDER SKYBOX (end)

    // RENDER OBJECTS (PIECE, CHESSBOARD)
    m_basic_shader.use();
    m_basic_shader.set_uniform_matrix_4fv("view", m_camera.get_view_matrix());
    m_basic_shader.set_uniform_matrix_4fv("projection", projection);

    // Lights
    m_basic_shader.set_uniform_3fv("lightPos1", glm::vec3(5.0f, 5.0f, 5.0f));
    m_basic_shader.set_uniform_3fv("lightColor1", glm::vec3(1.0f, 1.0f, 1.0f));

    // Update mobile light
    float lightSpeed = 0.5f;
    float radius     = 5.0f;
    // Mobile light animation
    glm::vec3 lightPos2   = glm::vec3(radius * sin(elapsed_time * lightSpeed), 5.0f, radius * cos(elapsed_time * lightSpeed));
    glm::vec3 lightColor2 = glm::vec3(1.0f, 1.0f, 1.0f);
    m_basic_shader.set_uniform_3fv("lightPos2", lightPos2);
    m_basic_shader.set_uniform_3fv("lightColor2", lightColor2);

    m_basic_shader.set_uniform_3fv("viewPos", m_camera.get_position());

    // UPDATE
    if (move_processing.has_value())
    {
        if (!animation.is_animating)
        {
            animation.is_animating         = true;
            animation.animation_start_time = elapsed_time;
        }

        // -> ANIMATION
        m_game_object_manager.move_piece(chessboard, move_processing.value(), elapsed_time, animation);

        // -> Update data after the animation's end
        if (!animation.is_animating)
        {
            std::cout << "Update data after the end of animation!"
                      << "\n";
            m_game_object_manager.update_pieces_positions(chessboard);
            m_game_object_manager.update_pieces_data();
            move_processing.reset(); // reset the move processing after animation (from / to reinitialized)
        }
    }
    // RENDER OBJECTS (PIECE, CHESSBOARD)
    m_game_object_manager.render_game_objects(m_basic_shader, settings);
}

void Renderer3D::camera_position_and_orientation_listener(std::optional<int> selected_piece_index, Color current_player)
{
    if (selected_piece_index.has_value() && !m_camera.is_track_ball())
    {
        m_camera.set_camera_piece_position(Renderer3DUtils::world_position(Renderer3DUtils::get_position(*selected_piece_index)));

        if (current_player == Color::White)
        {
            m_camera.active_camera_piece_view(-90.0f);
        }
        else
        {
            m_camera.active_camera_piece_view(90.0f);
        }
    }
    else
    {
        m_camera.active_camera_track_ball();
    }
};
