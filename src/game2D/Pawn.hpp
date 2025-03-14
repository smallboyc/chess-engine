#pragma once
#include <optional>
#include "Piece.hpp"

struct EnPassantMove {
    int pawn_index_to_capture;
    int next_move_after_capture;
};

class Pawn : public Piece {
public:
    explicit Pawn(const Color color = Color::None)
        : Piece(Type::Pawn, color) {}
    void set_legal_moves(int from, const std::array<std::unique_ptr<Piece>, 64>& board, Turn& turn) override;
    void move_piece(int from, int to, std::array<std::unique_ptr<Piece>, 64>& board, Turn& turn) override;

private:
    bool is_vulnerable_to_en_passant() const override { return m_vulnerable_to_en_passant; };
    void set_active_double_move() override { m_double_move = true; };
    bool has_target_enemy_at_same_level(int from, int to, std::array<std::unique_ptr<Piece>, 64>& board);
    //
    std::optional<EnPassantMove> m_en_passant{std::nullopt};
    bool                         m_vulnerable_to_en_passant{false};
    bool                         m_double_move{true};
};


//utils
bool is_at_the_board_end(int to);
void enable_transformation(int from, int to, std::array<std::unique_ptr<Piece>, 64>& board,Turn& turn);