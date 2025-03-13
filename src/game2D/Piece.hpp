#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "glm/fwd.hpp"

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



inline std::string getPieceType(const Type& type)
{
    if (type == Type::Pawn)
        return "Pawn";
    if (type == Type::Knight)
        return "Knight";
    if (type == Type::Bishop)
        return "Bishop";
    if (type == Type::Rook)
        return "Rook";
    if (type == Type::Queen)
        return "Queen";
    if (type == Type::King)
        return "King";
    return "None";
}

inline std::string getColor(const glm::vec3& color)
{
    if (color == glm::vec3(0.961, 0.859, 0.635))
        return "White";
    if (color == glm::vec3(0.086f, 0.086f, 0.129f))
        return "Black";
}

class Piece {
public:
    explicit Piece(const Type type = Type::None, const Color color = Color::None)
        : m_type(type), m_color(color){};
    virtual ~Piece()               = default;
    Piece(const Piece&)            = delete;
    Piece& operator=(const Piece&) = delete;
    Piece(Piece&&)                 = default;
    Piece&    operator=(Piece&&)   = default;
    glm::vec3 getColor() const { return m_color == Color::White ? glm::vec3(0.961, 0.859, 0.635) : glm::vec3(0.086f, 0.086f, 0.129f); };
    Type      get_type() const { return m_type; };

private:
    Type  m_type;
    Color m_color;
};

class Pawn : public Piece {
public:
    explicit Pawn(const Color color)
        : Piece(Type::Pawn, color) {}
};

class Knight : public Piece {
public:
    explicit Knight(const Color color)
        : Piece(Type::Knight, color) {}
};

class Bishop : public Piece {
public:
    explicit Bishop(const Color color)
        : Piece(Type::Bishop, color) {}
};

class Rook : public Piece {
public:
    explicit Rook(const Color color)
        : Piece(Type::Rook, color) {}
};

class Queen : public Piece {
public:
    explicit Queen(const Color color)
        : Piece(Type::Queen, color) {}
};

class King : public Piece {
public:
    explicit King(const Color color)
        : Piece(Type::King, color) {}
};
