#pragma once

#include "renderer3D/Animation.hpp"
#include "game2D/ChessGame.hpp"
#include "gui/Settings.hpp"

class Game2DWrapper {
public:
    Game2DWrapper()  = default;
    ~Game2DWrapper() = default;
    void wrap(ChessGame& chess_game, Settings& settings, Animation& animation, const char* wrapper_name = "Chessboard");
};