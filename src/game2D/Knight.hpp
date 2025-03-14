#pragma once
#include "Piece.hpp"

class Knight : public Piece {
public:
    explicit Knight(const Color color = Color::None)
        : Piece(Type::Knight, color) {}
    void set_legal_moves(int from, const std::array<std::unique_ptr<Piece>, 64>& board, Turn& turn) override;
};
