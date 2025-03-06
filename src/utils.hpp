#pragma once
#include <glm/glm.hpp>
#include <vector>

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

struct PiecePositions {
    Type             piece_type;
    std::vector<int> black_position;
    std::vector<int> white_position;
};

inline glm::vec2 get_position(const int index)
{
    return {index % 8, index / 8};
};

inline glm::vec3 world_position(glm::vec2 position)
{
    return {-3.5f + position.x, 0.0f, -3.5f + position.y};
}