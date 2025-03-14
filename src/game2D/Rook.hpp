#pragma once
#include "Piece.hpp"


class Rook : public Piece {
public:
    explicit Rook(const Color color = Color::None)
        : Piece(Type::Rook, color) {}
    void set_legal_moves(int from, const std::array<std::unique_ptr<Piece>, 64>& board, Turn& turn) override;
};