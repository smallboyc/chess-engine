#pragma once
#include <chrono>
#include <glm/glm.hpp>
#include <quick_imgui/quick_imgui.hpp>
#include "Camera.hpp"
#include "GameObjectManager.hpp"
#include "Shader.hpp"
#include "renderer3D/Skybox.hpp"

class Renderer3D {
public:
    Renderer3D(int width, int height)
        : window_width(width), window_height(height), start_time(std::chrono::steady_clock::now()){};
    glmax::Camera& use_camera() { return m_camera; };
    void           window_size_callback(int width, int height);
    void           toggle_active_camera_callback(int key, int action);
    void           init(std::array<std::unique_ptr<Piece>, 64>& chessboard);
    void           run(std::array<std::unique_ptr<Piece>, 64>& chessboard, std::optional<MoveProcessing>& move_processing, Animation& animation, Settings& Settings);

private:
    glmax::Camera                         m_camera{true};
    glmax::Shader                         m_basic_shader{};
    glmax::Shader                         m_skybox_shader{};
    GameObjectManager                     m_game_object_manager;
    Skybox                                m_skybox;
    int                                   window_width;
    int                                   window_height;
    std::chrono::steady_clock::time_point start_time;
};