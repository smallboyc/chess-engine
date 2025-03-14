#pragma once
#include <array>
#include "utils.hpp"

const std::array<Move, 8> king_moves = {
    {{Direction::Left, Direction::Up},
     {Direction::None, Direction::Up},
     {Direction::Right, Direction::Up},
     {Direction::Right, Direction::None},
     {Direction::Right, Direction::Down},
     {Direction::None, Direction::Down},
     {Direction::Left, Direction::Down},
     {Direction::Left, Direction::None}}
};

const std::array<Move, 8> knight_moves = {
    {{Direction::Left, Direction::KnightUp},
     {Direction::Right, Direction::KnightUp},
     {Direction::KnightLeft, Direction::Up},
     {Direction::KnightRight, Direction::Up},
     {Direction::Right, Direction::KnightDown},
     {Direction::Left, Direction::KnightDown},
     {Direction::KnightRight, Direction::Down},
     {Direction::KnightLeft, Direction::Down}}
};

const std::array<Move, 4> bishop_moves = {
    {{Direction::Left, Direction::Up},
     {Direction::Right, Direction::Down},
     {Direction::Right, Direction::Up},
     {Direction::Left, Direction::Down}}
};

const std::array<Move, 4> rook_moves = {
    {{Direction::Left, Direction::None},
     {Direction::Right, Direction::None},
     {Direction::None, Direction::Up},
     {Direction::None, Direction::Down}
    }
};

const std::array<Move, 8> queen_moves = {
    {{Direction::Left, Direction::None},
     {Direction::Right, Direction::None},
     {Direction::None, Direction::Up},
     {Direction::None, Direction::Down},
     {Direction::Left, Direction::Up},
     {Direction::Right, Direction::Down},
     {Direction::Right, Direction::Up},
     {Direction::Left, Direction::Down}}
};
