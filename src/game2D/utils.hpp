#pragma once
#include <glfw/src/internal.h>
#include <imgui.h>
#include <optional>
#include "Types.hpp"
#include "glm/fwd.hpp"
#include "stb_image.h"

struct Position {
    int x{};
    int y{};
};

enum class Direction {
    Up          = -1,
    Down        = 1,
    Left        = -1,
    Right       = 1,
    None        = 0,
    KnightUp    = -2,
    KnightDown  = 2,
    KnightLeft  = 2,
    KnightRight = -2,
};

struct Move {
    Direction dx{};
    Direction dy{};
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

struct MoveStatus {
    int     index;
    Texture texture;
    bool    capture;
};

struct Turn {
    int                total{0};
    Color              current_player{Color::White};
    std::optional<int> pawn_capture;
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
