#include "Pawn.hpp"
#include <imgui.h>
#include <array>
#include <memory>
#include <optional>
#include "Piece.hpp"
#include "utils.hpp"

void Pawn::set_legal_moves(int from, const Chessboard& board, Turn& turn)
{
    Position position  = get_position(from);
    int      max_range = direction();
    int      next_step = position.y + direction();

    if (m_double_move)
        max_range += direction();

    for (int y{next_step}; y != next_step + max_range; y += direction())
    {
        int cell_index_to_check = get_index({position.x, y});
        if (!is_empty_cell(cell_index_to_check, board))
            break;
        legal_moves().push_back(cell_index_to_check);
    }

    std::optional<int> left_diagonal_index  = (position.x > 0) ? std::make_optional(get_index({position.x - 1, position.y + direction()})) : std::nullopt;
    std::optional<int> right_diagonal_index = (position.x < 7) ? std::make_optional(get_index({position.x + 1, position.y + direction()})) : std::nullopt;

    for (std::optional<int> potential_enemy_index : {left_diagonal_index, right_diagonal_index})
    {
        if (potential_enemy_index && is_enemy_targeted(*potential_enemy_index, turn, board))
        {
            legal_moves().push_back(*potential_enemy_index);
        }
    }

    std::optional<int> left_enemy_index  = (position.x > 0) ? std::make_optional(get_index({position.x - 1, position.y})) : std::nullopt;
    std::optional<int> right_enemy_index = (position.x < 7) ? std::make_optional(get_index({position.x + 1, position.y})) : std::nullopt;

    for (std::optional<int> potential_enemy_index : {left_enemy_index, right_enemy_index})
    {
        if (!potential_enemy_index)
            continue;

        int enemy_index = *potential_enemy_index;
        if (is_enemy_targeted(enemy_index, turn, board) && board[enemy_index]->is_vulnerable_to_en_passant() && turn.pawn_capture == turn.total)
        {
            Position enemy_position         = get_position(enemy_index);
            Position after_capture_position = {enemy_position.x, enemy_position.y + direction()};

            if (after_capture_position.y >= 0 && after_capture_position.y < 8)
            {
                int index_after_en_passant = get_index(after_capture_position);
                legal_moves().push_back(index_after_en_passant);
                m_en_passant = {enemy_index, index_after_en_passant};
            }
        }
    }
}

void Pawn::move_piece(const int from, const int to, Chessboard& board, Turn& turn, std::optional<MoveProcessing>& move_processing)
{
    if (is_at_the_board_end(to))
    {
        turn.pawn_promotion = {from, to};
    }
    else
    {
        if (turn.total == turn.pawn_capture && to == m_en_passant->next_move_after_capture)
        {
            board[m_en_passant->pawn_index_to_capture].reset();
            m_vulnerable_to_en_passant = false;
        }
        turn.total++;
        if (m_double_move)
        {
            if (has_target_enemy_at_same_level(from, to, board))
            {
                turn.pawn_capture          = turn.total;
                m_vulnerable_to_en_passant = true;
            }
            m_double_move = false;
        }
        move_processing = {from, to};
    }
    //
    board[to] = std::move(board[from]);
}

bool Pawn::has_target_enemy_at_same_level(int from, int to, Chessboard& board)
{
    return board[to - 1] != nullptr && board[to - 1]->get_color() != board[from]->get_color() || board[to + 1] != nullptr && board[to + 1]->get_color() != board[from]->get_color();
}

bool is_at_the_board_end(int to)
{
    Position position = get_position(to);
    return position.y == 7 || position.y == 0;
}
