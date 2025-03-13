#pragma once
#include "Piece.hpp"

class Chessboard {
public:
    explicit Chessboard(std::array<PiecePositions, 6> initial_positions);
    std::array<std::unique_ptr<Piece>, 64> m_board;
    void                                   set_piece_on_board(const PiecePositions& piece_positions, const Color& piece_color);
    std::unique_ptr<Piece>                 create_piece(const Type& piece_type, const Color& piece_color);
};
