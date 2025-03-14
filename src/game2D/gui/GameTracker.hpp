#pragma once
#include "../Chessboard.hpp"

class GameTracker {
public:
    GameTracker() = default;
    void show(Chessboard& chessboard);

private:
    void history_moves_widget(Chessboard& chessboard, std::vector<MoveStatus>& all_moves);
    void selected_piece_widget(Chessboard& chessboard);
    void warnings_widget(Chessboard &chessboard);
};
