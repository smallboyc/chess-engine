#include "Bishop.hpp"
#include <imgui.h>
#include "Piece.hpp"
#include "game2D/Types.hpp"
#include "utils.hpp"

void Bishop::set_legal_moves(int from, const Chessboard& board, Turn& turn)
{
    for (const Move& move : bishop_moves)
    {
        add_moves_in_direction(from, {move.dx, move.dy}, legal_moves(), turn, board);
    }
}