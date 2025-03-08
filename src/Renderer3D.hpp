#pragma once
#include <chrono>
#include <glm/glm.hpp>
#include <quick_imgui/quick_imgui.hpp>
#include "Camera.hpp"
#include "Chessboard.hpp"
#include "PieceManager.hpp"
#include "Shader.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

class Renderer3D {
public:
    Renderer3D(int width, int height)
        : window_width(width), window_height(height), window("Quick ImGui", [&]() {
            shader.loadShader("model.vs.glsl", "model.fs.glsl");
            piece_manager.init(_chessboard.m_board);
            piece_manager.loadAllMesh();
            piece_manager.m_chessboard.m_modelMatrices.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
            piece_manager.m_chessboard.loadMesh("chessboard/chessboard.obj", "chessboard");
            piece_manager.m_chessboard.setupBuffers();
        })
        , start_time(std::chrono::steady_clock::now())
    {
        initCallbacks();
    };

    void initCallbacks();
    void run();

private:
    void updatePiecePosition(float elapsedTime);

    glmax::Camera                         Camera{true};
    Chessboard                            _chessboard;
    unsigned int                          from = 49;
    unsigned int                          to   = 33;
    glmax::Shader                         shader{};
    PieceManager                          piece_manager;
    quick_imgui::WindowWrapper            window;
    int                                   window_width;
    int                                   window_height;
    bool                                  isAnimating{};
    float                                 animationStartTime{0.0f};
    float                                 animationDuration{2.0f};
    bool                                  isFirstTime{true};
    std::chrono::steady_clock::time_point start_time;
};