#include "Piece.hpp"
#include <iostream>
#include "Types.hpp"
#include "utils.hpp"

Texture Piece::get_texture(const Textures& textures) const
{
    return textures.at(get_path() + ".png");
}

std::string Piece::get_path() const
{
    switch (m_type)
    {
    case Type::King:
        return m_color == Color::Black ? "bk" : "wk";
    case Type::Queen:
        return m_color == Color::Black ? "bq" : "wq";
    case Type::Bishop:
        return m_color == Color::Black ? "bb" : "wb";
    case Type::Knight:
        return m_color == Color::Black ? "bn" : "wn";
    case Type::Rook:
        return m_color == Color::Black ? "br" : "wr";
    case Type::Pawn:
        return m_color == Color::Black ? "bp" : "wp";
    default:
        return "null";
    }
}

int Piece::direction() const
{
    return get_color() == Color::Black ? 1 : -1;
}

bool is_empty_cell(const int index, const Chessboard& board)
{
    return board[index] == nullptr;
}

bool is_enemy_targeted(const int index, Turn& turn, const Chessboard& board)
{
    return !is_empty_cell(index, board) && turn.current_player != board[index]->get_color();
}

void add_moves_in_direction(int from, const Move move, std::vector<int>& legal_moves, Turn& turn, const Chessboard& board)
{
    Position position = get_position(from);
    int      x        = position.x + static_cast<int>(move.dx);
    int      y        = position.y + static_cast<int>(move.dy);

    while (x >= 0 && x < 8 && y >= 0 && y < 8)
    {
        int cell_index_to_check = get_index({x, y});
        if (is_enemy_targeted(cell_index_to_check, turn, board))
        {
            legal_moves.push_back(cell_index_to_check);
            break;
        }
        else if (!is_empty_cell(cell_index_to_check, board))
        {
            break;
        }
        legal_moves.push_back(cell_index_to_check);

        if (board[from]->get_type() == Type::Knight || board[from]->get_type() == Type::King)
            break;

        x += static_cast<int>(move.dx);
        y += static_cast<int>(move.dy);
    }
}

bool Piece::player_move_is_legal(int choice)
{
    if (!m_legal_moves.empty())
        return std::find(m_legal_moves.begin(), m_legal_moves.end(), choice) != m_legal_moves.end();
    return false;
}

void Piece::move_piece(const int from, const int to, Chessboard& board, Turn& turn, std::optional<MoveProcessing>& move_processing)
{
    turn.total++;
    board[to]   = std::move(board[from]);
    board[from] = nullptr;
    if (!m_has_moved)
        m_has_moved = true;
    move_processing = {from, to};
}

void Piece::draw_scopes(int cell_index, Turn& turn, const Chessboard& board)
{
    if (std::find(m_legal_moves.begin(), m_legal_moves.end(), cell_index) != m_legal_moves.end())
    {
        if (is_enemy_targeted(cell_index, turn, board))
            draw_scope(IM_COL32(0, 255, 255, 255));
        else
            draw_scope(IM_COL32(0, 255, 0, 255));
    }
}