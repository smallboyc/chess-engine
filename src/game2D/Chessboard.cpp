
#include "Chessboard.hpp"

/*
 0  1  2  3  4  5  6  7
 8  9 10 11 12 13 14 15
16 17 18 19 20 21 22 23
24 25 26 27 28 29 30 31
32 33 34 35 36 37 38 39
40 41 42 43 44 45 46 47
48 49 50 51 52 53 54 55
56 57 58 59 60 61 62 63
*/

Chessboard::Chessboard(std::array<PiecePositions, 6> initial_positions)
{
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