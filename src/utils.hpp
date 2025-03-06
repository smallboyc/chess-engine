#pragma once
#include <glm/glm.hpp>
enum class Type {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    CHESSBOARD,
};

inline glm::vec2 get_position(const int index)
{
    return {index % 8, index / 8};
};

inline glm::vec3 world_position(glm::vec2 position)
{
    return {-3.5f + position.x, 0.0f, -3.5f + position.y};
}