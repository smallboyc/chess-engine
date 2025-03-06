
#include "Chessboard.hpp"

Chessboard::Chessboard()
{
    std::array<PiecePositions, 6> initial_positions = {
        PiecePositions{Type::Pawn, {8, 9, 10, 11, 12, 13, 14, 15}, {48, 49, 51, 50, 52, 53, 54, 55}},
        PiecePositions{Type::Rook, {0, 7}, {56, 63}},
        PiecePositions{Type::Knight, {1, 6}, {57, 62}},
        PiecePositions{Type::Bishop, {2, 5}, {58, 61}},
        PiecePositions{Type::Queen, {3}, {59}},
        PiecePositions{Type::King, {4}, {60}}
    };

    for (const PiecePositions& piece_positions : initial_positions)
    {
        set_piece_on_board(piece_positions, Color::White);
        set_piece_on_board(piece_positions, Color::Black);
    }
}

void Chessboard::set_piece_on_board(const PiecePositions& piece_positions, const Color& piece_color)
{
    std::vector<int> current_piece_color = piece_positions.white_position;
    if (piece_color == Color::Black)
        current_piece_color = piece_positions.black_position;

    for (const int& piece_position : current_piece_color)
        m_board[piece_position] = create_piece(piece_positions.piece_type, piece_color);
}

std::unique_ptr<Piece> Chessboard::create_piece(const Type& piece_type, const Color& piece_color)
{
    switch (piece_type)
    {
    case Type::Pawn: return std::make_unique<Pawn>(piece_color);
    case Type::Knight: return std::make_unique<Knight>(piece_color);
    case Type::Bishop: return std::make_unique<Bishop>(piece_color);
    case Type::Rook: return std::make_unique<Rook>(piece_color);
    case Type::King: return std::make_unique<King>(piece_color);
    case Type::Queen: return std::make_unique<Queen>(piece_color);
    default: return nullptr;
    }
}