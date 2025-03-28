#pragma once
#include <glfw/src/internal.h>
#include <imgui.h>
#include <optional>
#include "Types.hpp"
#include "stb_image.h"

struct Position {
    int x{};
    int y{};
};

enum class Color {
    White,
    Black,
    None,
};

enum class Type {
    King,
    Queen,
    Bishop,
    Knight,
    Rook,
    Pawn,
    None,
};

// Different game status.
struct Status {
    bool free_play{true};
    bool king_is_selected{};
    bool waiting_for_king_move{};
    bool defender_is_selected{};
    bool waiting_for_defender_move{};
    void reset();
};

struct Warnings {
    bool check{};
    bool checkmate{};
    bool dangerous_move{};
    void reset();
};

// gui
struct MoveStatus {
    int     index;
    Texture texture;
    bool    capture;
};

struct PawnPromotion {
    int from{};
    int to{};
};

struct Turn {
    int                          total{0};
    Color                        current_player{Color::White};
    std::optional<int>           pawn_capture;
    std::optional<PawnPromotion> pawn_promotion; // pawn transform
};

Position              get_position(int index);
Position              get_distance(Position& position_1, Position& position_2);
int                   get_index(Position position);
void                  draw_scope(ImU32 scope_color);
void                  load_font(const std::string& path, float size);
GLuint                load_texture_from_file(const std::string& path);
std::pair<char, char> get_board_coordinate(Position position);
bool                  are_equal(const ImVec4& a, const ImVec4& b);
std::string           player_turn(Color color);
