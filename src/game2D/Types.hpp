#pragma once

#include <glfw/src/internal.h>
#include <unordered_map>

// Textures
using Texture  = GLuint;
using Textures = std::unordered_map<std::string, Texture>;

// Chessboard
class Piece;
using Chessboard = std::array<std::unique_ptr<Piece>, 64>;