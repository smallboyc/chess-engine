#include "Queen.hpp"
#include <imgui.h>
#include "Piece.hpp"

void Queen::set_legal_moves(int from, const Chessboard& board, Turn& turn)
{
    for (const Move& move : queen_moves)
    {
        add_moves_in_direction(from, {move.dx, move.dy}, legal_moves(), turn, board);
    }
}