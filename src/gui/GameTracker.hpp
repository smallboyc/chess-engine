#pragma once
#include "../game2D/ChessGame.hpp"

class GameTracker {
public:
    GameTracker() = default;
    void show(ChessGame& chess_game);

private:
    void history_moves_widget(ChessGame& chess_game, std::vector<MoveStatus>& all_moves);
    void selected_piece_widget(ChessGame& chess_game);
    void warnings_widget(ChessGame& chess_game);
};
