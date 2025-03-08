#pragma once
#include <chrono>
#include <glm/glm.hpp>
#include <quick_imgui/quick_imgui.hpp>
#include "Camera.hpp"
#include "GameObjectManager.hpp"
#include "Shader.hpp"
#include "game2D/Chessboard.hpp"

class Renderer3D {
public:
    Renderer3D(int width, int height)
        : window_width(width), window_height(height), window("Quick ImGui", [&]() {
            // init shader
            shader.loadShader("model.vs.glsl", "model.fs.glsl");
            // init all pieces
            game_object_manager.initPiecesPositions(_chessboard.m_board);
            game_object_manager.loadAllPieces();
            // init chessboard
            game_object_manager.loadChessboard();
        })
        , start_time(std::chrono::steady_clock::now())
    {
        initCallbacks();
    };

    void initCallbacks();
    void run();

private:
    void update(float elapsedTime);
    void render();

    glmax::Camera              Camera{true};
    glmax::Shader              shader{};
    Chessboard                 _chessboard;
    GameObjectManager          game_object_manager;
    quick_imgui::WindowWrapper window;
    int                        window_width;
    int                        window_height;
    bool                       isAnimating{};
    float                      animationStartTime{0.0f};
    // Animation test with knight positions
    unsigned int                          from = 57;
    unsigned int                          to   = 42;
    std::chrono::steady_clock::time_point start_time;
};