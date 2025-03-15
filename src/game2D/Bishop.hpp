#pragma once
#include "Piece.hpp"

class Bishop : public Piece {
public:
    explicit Bishop(const Color color = Color::None)
        : Piece(Type::Bishop, color) {}
    void set_legal_moves(int from, const Chessboard& board, Turn& turn) override;
};