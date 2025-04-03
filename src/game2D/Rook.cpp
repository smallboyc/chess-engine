#include "Rook.hpp"
#include "Piece.hpp"
#include "utils.hpp"

void Rook::set_legal_moves(int from, const Chessboard& board, Turn& turn)
{
    for (const Move& move : rook_moves)
    {
        add_moves_in_direction(from, {move.dx, move.dy}, legal_moves(), turn, board);
    }
}