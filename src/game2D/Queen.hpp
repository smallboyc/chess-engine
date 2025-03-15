#pragma once
#include "Piece.hpp"

class Queen : public Piece {
public:
    explicit Queen(const Color color = Color::None)
        : Piece(Type::Queen, color) {}
    void set_legal_moves(int from, const Chessboard& board, Turn& turn) override;
};