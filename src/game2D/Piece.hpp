#pragma once
#include <imgui.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "PieceMove.hpp"
#include "Types.hpp"
#include "utils.hpp"

// Use to pass the move data to the Renderer3D & perform a cool animation.
struct MoveProcessing {
    int from;
    int to;
};

struct PiecePositions {
    Type             piece_type;
    std::vector<int> black_position;
    std::vector<int> white_position;
};

class Piece {
public:
    explicit Piece(const Type type = Type::None, const Color color = Color::None)
        : m_type(type), m_color(color){};
    virtual ~Piece()               = default;
    Piece(const Piece&)            = delete;
    Piece& operator=(const Piece&) = delete;
    Piece(Piece&&)                 = default;
    Piece& operator=(Piece&&)      = default;

    Type        get_type() const { return m_type; };
    glm::vec3   get_vec_color() const { return m_color == Color::White ? glm::vec3(0.961, 0.859, 0.635) : glm::vec3(0.086f, 0.086f, 0.129f); };
    Color       get_color() const { return m_color; };
    Texture     get_texture(const Textures& textures) const;
    std::string get_path() const;
    //
    int               direction() const;
    std::vector<int>& legal_moves() { return m_legal_moves; };
    bool              player_move_is_legal(int choice);
    bool              has_moved() const { return m_has_moved; }
    void              set_has_moved(bool has_moved) { m_has_moved = has_moved; };
    void              draw_scopes(int cell_index, Turn& turn, const Chessboard& board);
    void              set_capture(bool capture) { m_has_captured = capture; };
    bool              has_captured() const { return m_has_captured; };
    //
    virtual bool is_vulnerable_to_en_passant() const { return false; };
    virtual void set_active_double_move(){};
    virtual void bind_rooks(std::vector<int>& rooks){};
    virtual void set_legal_moves(int from, const Chessboard& board, Turn& turn) = 0;
    virtual void move_piece(int from, int to, Chessboard& board, Turn& turn, std::optional<MoveProcessing>& move_processing);

private:
    Type             m_type;
    Color            m_color;
    std::vector<int> m_legal_moves;
    bool             m_has_captured{};
    bool             m_has_moved{};
};

bool is_empty_cell(int index, const Chessboard& board);
bool is_enemy_targeted(int index, Turn& turn, const Chessboard& board);
void add_moves_in_direction(int from, Move move, std::vector<int>& legal_moves, Turn& turn, const Chessboard& board);
