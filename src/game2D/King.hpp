#pragma once
#include <optional>
#include "Piece.hpp"

struct SpecialMove {
    int new_king_index;
    int new_rook_index;
};
struct Castling {
    explicit Castling(const std::vector<int>& rooks)
        : queenside_rook_index(rooks[0]), kingside_rook_index(rooks[1]) {}
    //
    int         queenside_rook_index{};
    bool        queenside_canceled{};
    int         kingside_rook_index{};
    bool        kingside_canceled{};
    SpecialMove kingside_special_moves{};
    SpecialMove queenside_special_moves{};
};

class King : public Piece {
public:
    explicit King(const Color color = Color::None)
        : Piece(Type::King, color) {}
    void set_legal_moves(int from, const std::array<std::unique_ptr<Piece>, 64>& board, Turn& turn) override;
    void move_piece(int from, int to, std::array<std::unique_ptr<Piece>, 64>& board, Turn& turn) override;
    //
    void add_threatening_pieces_in_direction(int start, Move move, Turn& turn, std::unordered_map<Type, std::vector<int>>& threats, const std::vector<Type>& targets, const std::array<std::unique_ptr<Piece>, 64>& board);
    void add_threatening_pieces(int start, Turn& turn, std::unordered_map<Type, std::vector<int>>& threats, const std::array<std::unique_ptr<Piece>, 64>& board);
    bool is_in_check(int index, Turn& turn, const std::array<std::unique_ptr<Piece>, 64>& board);
    bool is_double_check(int index, Turn& turn, const std::array<std::unique_ptr<Piece>, 64>& board);
    bool has_allies_to_defend(Turn& turn, const std::array<std::unique_ptr<Piece>, 64>& board);
    bool can_escape(int index, Turn& turn, const std::array<std::unique_ptr<Piece>, 64>& board);
    void reset_buffers();
    //
    auto&            get_threats() const { return m_threats; };
    auto             get_defenders() const { return m_defenders; };
    std::vector<int> get_escape_moves() { return m_escape_moves; };
    //
    void draw_threat_scopes(int index, int cell_index);
    void draw_defenders_scopes(std::optional<std::pair<int, Piece*>>& selected_piece, int cell_index);

    // Castling
    void                    set_castling(const std::vector<int>& rooks) { m_castling = Castling(rooks); };
    std::optional<Castling> get_castling() const { return m_castling; }
    void                    check_for_castling(int index, const std::array<std::unique_ptr<Piece>, 64>& board, Turn& turn);
    bool                    kingside_castling_is_free(int index, const std::array<std::unique_ptr<Piece>, 64>& board, Turn& turn);
    bool                    queenside_castling_is_free(int index, const std::array<std::unique_ptr<Piece>, 64>& board, Turn& turn);
    bool                    cell_is_free(int i, const std::array<std::unique_ptr<Piece>, 64>& board, Turn& turn);
    bool                    handle_castling(int king_target, std::array<std::unique_ptr<Piece>, 64>& board);
    void                    move_rook(int from, int to, std::array<std::unique_ptr<Piece>, 64>& board);

private:
    std::optional<Castling> m_castling;
    // buffers
    std::vector<int>                           m_escape_moves;
    std::unordered_map<Type, std::vector<int>> m_threats;
    std::unordered_map<int, std::vector<int>>  m_defenders;
    //
};