#pragma once
#include <chrono>
#include <glm/glm.hpp>
#include <quick_imgui/quick_imgui.hpp>
#include "Camera.hpp"
#include "GameObjectManager.hpp"
#include "Shader.hpp"
#include "game2D/Chessboard.hpp"
#include "game2D/Piece.hpp"

class Renderer3D {
public:
    Renderer3D(int width, int height)
        : window_width(width), window_height(height), start_time(std::chrono::steady_clock::now()){};
    glmax::Camera& useCamera() { return m_camera; };
    void           window_size_callback(int width, int height);
    void           delete_piece_callback(int key, int action);
    void           init();
    void           run();

private:
    std::array<PiecePositions, 6> initial_positions = {
        PiecePositions{Type::Pawn, {8, 9, 10, 11, 12, 13, 14, 15}, {48, 49, 51, 50, 52, 53, 54, 55}},
        PiecePositions{Type::Rook, {0, 7}, {56, 63}},
        PiecePositions{Type::Knight, {1, 6}, {57, 62}},
        PiecePositions{Type::Bishop, {2, 5}, {58, 61}},
        PiecePositions{Type::Queen, {3}, {59}},
        PiecePositions{Type::King, {4}, {60}}
    };
    void              update(float elapsedTime);
    void              render(float elapsedTime);
    glmax::Camera     m_camera{true};
    glmax::Shader     m_shader{};
    Chessboard        _chessboard{initial_positions}; //temporary (test)
    GameObjectManager m_gameObjectManager;
    int               window_width;
    int               window_height;
    bool              isAnimating{};
    float             animationStartTime{0.0f};
    // Animation test with knight positions
    unsigned int                          from = 1;
    unsigned int                          to   = 17;
    std::chrono::steady_clock::time_point start_time;
    bool                                  isPieceDeleted{false};
};