#include "King.hpp"
#include <iostream>
#include <span>
#include <unordered_map>
#include <vector>
#include "PieceMove.hpp"
#include "utils.hpp"

void King::set_legal_moves(int from, const Chessboard& board, Turn& turn)
{
    std::vector<int> theoric_moves;
    for (const Move& move : king_moves)
    {
        add_moves_in_direction(from, {move.dx, move.dy}, theoric_moves, turn, board);
    }
    for (const int& move : theoric_moves)
    {
        std::unordered_map<Type, std::vector<int>> threatened_move;
        add_threatening_pieces(move, turn, threatened_move, board);
        if (threatened_move.empty())
        {
            legal_moves().push_back(move);
        }
    }
}

void King::move_piece(const int from, const int to, Chessboard& board, Turn& turn, std::optional<MoveProcessing>& move_processing)
{
    turn.total++;
    board[to] = std::move(board[from]);

    if (m_castling)
    {
        if (handle_castling(to, board))
        {
            std::cout << "Roque!"
                      << "\n";
            m_castling.reset();
        }
    }

    if (!has_moved())
        set_has_moved(true);
    //
    move_processing = {from, to};
}

bool King::handle_castling(int king_target, Chessboard& board)
{
    if (king_target == m_castling->kingside_special_moves.new_king_index)
    {
        move_rook(m_castling->kingside_rook_index, m_castling->kingside_special_moves.new_rook_index, board);
        return true;
    }
    if (king_target == m_castling->queenside_special_moves.new_king_index)
    {
        move_rook(m_castling->queenside_rook_index, m_castling->queenside_special_moves.new_rook_index, board);
        return true;
    }
    return false;
}

void King::move_rook(int from, int to, Chessboard& board)
{
    board[to]   = std::move(board[from]);
    board[from] = nullptr;
}

// King
void King::add_threatening_pieces_in_direction(int start, const Move move, Turn& turn, std::unordered_map<Type, std::vector<int>>& threats, const std::vector<Type>& targets, const Chessboard& board)
{
    Position position = get_position(start);
    int      x        = position.x + static_cast<int>(move.dx);
    int      y        = position.y + static_cast<int>(move.dy);

    std::vector<int> possible_indexes;

    while (x >= 0 && x < 8 && y >= 0 && y < 8)
    {
        int cell_index_to_check = get_index({x, y});

        possible_indexes.push_back(cell_index_to_check);

        if (!is_empty_cell(cell_index_to_check, board))
        {
            Type pieceType = board[cell_index_to_check]->get_type();
            if (std::find(targets.begin(), targets.end(), pieceType) != targets.end() && is_enemy_targeted(cell_index_to_check, turn, board))
            {
                threats.insert({pieceType, possible_indexes});
            }
            break; // On arrête après la première pièce rencontrée
        }

        // Arrêt immédiat pour les pièces qui ne vont qu'à une case
        if (targets.size() == 1 && (targets[0] == Type::King || targets[0] == Type::Pawn || targets[0] == Type::Knight))
            break;

        x += static_cast<int>(move.dx);
        y += static_cast<int>(move.dy);
    }
}

struct TargetPieceTypeOnPath {
    std::span<const Move> path;
    std::vector<Type>     piece_types;
};

void King::add_threatening_pieces(int start, Turn& turn, std::unordered_map<Type, std::vector<int>>& threats, const Chessboard& board)
{
    std::vector<TargetPieceTypeOnPath> target_path_pieces = {
        {rook_moves, {Type::Rook, Type::Queen}},
        {bishop_moves, {Type::Bishop, Type::Queen}},
        {knight_moves, {Type::Knight}},
        {king_moves, {Type::King}},
    };

    for (const auto& target : target_path_pieces)
    {
        for (const Move& move : target.path)
        {
            add_threatening_pieces_in_direction(start, {move.dx, move.dy}, turn, threats, target.piece_types, board);
        }
    }
    Direction pawn_direction = Direction::Up;
    if (turn.current_player == Color::Black)
        pawn_direction = Direction::Down;

    add_threatening_pieces_in_direction(start, {Direction::Left, pawn_direction}, turn, threats, {Type::Pawn}, board);
    add_threatening_pieces_in_direction(start, {Direction::Right, pawn_direction}, turn, threats, {Type::Pawn}, board);
}

bool King::is_in_check(const int index, Turn& turn, const Chessboard& board)
{
    m_threats.clear();
    add_threatening_pieces(index, turn, m_threats, board);
    return !m_threats.empty();
};

bool King::is_double_check(const int index, Turn& turn, const Chessboard& board)
{
    return is_in_check(index, turn, board) && m_threats.size() > 1;
};

//
bool is_ally(int index, Turn& turn, const Chessboard& board)
{
    return !is_empty_cell(index, board) && board[index]->get_color() == turn.current_player && board[index]->get_type() != Type::King;
}

bool King::has_allies_to_defend(Turn& turn, const Chessboard& board)
{
    auto& [type, enemy_move] = *m_threats.begin();
    for (int i{0}; i < board.size(); i++)
    {
        if (is_ally(i, turn, board))
        {
            std::vector<int> ally_moves_ready_to_defend;
            board[i]->set_legal_moves(i, board, turn);
            for (const int legal_ally_move : board[i]->legal_moves())
            {
                if (std::find(enemy_move.begin(), enemy_move.end(), legal_ally_move) != enemy_move.end())
                {
                    ally_moves_ready_to_defend.push_back(legal_ally_move);
                }
            }
            board[i]->legal_moves().clear();
            if (!ally_moves_ready_to_defend.empty())
            {
                m_defenders.insert({i, ally_moves_ready_to_defend});
            }
        }
    }
    return !m_defenders.empty();
}

bool King::can_escape(const int index, Turn& turn, const Chessboard& board)
{
    // Récupérer les mouvements théoriques du roi.
    set_legal_moves(index, board, turn);
    if (legal_moves().empty()) // Aucune case accessible à côté du roi
    {
        return false;
    }
    // debug
    //  Récupérer les mouvements possibles de base du roi (vérifier si cases voisines ne sont pas attaquées)
    for (const int& move : legal_moves())
    {
        std::unordered_map<Type, std::vector<int>> escape_move_threatened;
        add_threatening_pieces(move, turn, escape_move_threatened, board);
        if (escape_move_threatened.empty())
        {
            m_escape_moves.push_back(move);
        }
    }
    legal_moves().clear();
    // Supprimer les indexes cachés par le roi engendré par un échec d'une pièce à longue portée.
    std::vector<int> forbidden_indexes{};
    for (const auto& [type, indexes] : m_threats)
    {
        if (type == Type::Queen || type == Type::Bishop || type == Type::Rook)
        {
            int      first_neighbour          = *indexes.begin();
            Position king_position            = get_position(index);
            Position first_neighbour_position = get_position(first_neighbour);
            Position distance                 = get_distance(king_position, first_neighbour_position);
            int      forbidden_index          = get_index({king_position.x + distance.x, king_position.y + distance.y});
            if (forbidden_index >= 0 && forbidden_index < 64)
            {
                forbidden_indexes.push_back(forbidden_index);
            }
        }
    }
    m_escape_moves.erase(
        std::remove_if(
            m_escape_moves.begin(), m_escape_moves.end(),
            [&](int move) { return std::find(forbidden_indexes.begin(), forbidden_indexes.end(), move) != forbidden_indexes.end(); }
        ),
        m_escape_moves.end()
    );
    return !m_escape_moves.empty();
}

void King::draw_threat_scopes(const int index, int cell_index)
{
    for (const auto& piece : m_threats)
    {
        if (std::find(piece.second.begin(), piece.second.end(), cell_index) != piece.second.end())
            draw_scope(IM_COL32(255, 0, 0, 255));
    }
    if (cell_index == index)
    {
        draw_scope(IM_COL32(255, 255, 0, 255));
    }
}
void King::draw_defenders_scopes(std::optional<std::pair<int, Piece*>>& selected_piece, int cell_index)
{
    for (const auto& [defender_index, defender_moves] : m_defenders)
    {
        if (defender_index == cell_index)
            draw_scope(IM_COL32(0, 0, 255, 255));
        if (selected_piece.has_value() && defender_index == selected_piece->first && std::find(defender_moves.begin(), defender_moves.end(), cell_index) != defender_moves.end())
        {
            draw_scope(IM_COL32(0, 255, 0, 255));
        }
    }
}

// Buffers
void King::reset_buffers()
{
    m_threats.clear();
    m_defenders.clear();
    m_escape_moves.clear();
}

// Castling
void King::check_for_castling(const int index, const Chessboard& board, Turn& turn)
{
    if (!m_castling->kingside_canceled)
    {
        if (!board[m_castling->kingside_rook_index])
            m_castling->kingside_canceled = true;
    }

    if (!m_castling->queenside_canceled)
    {
        if (!board[m_castling->queenside_rook_index])
            m_castling->queenside_rook_index = true;
    }

    if (has_moved())
    {
        m_castling.reset();
        return;
    }

    if (m_castling->kingside_canceled && m_castling->queenside_canceled)
    {
        m_castling.reset();
        return;
    }

    bool kingside_free  = kingside_castling_is_free(index, board, turn);
    bool queenside_free = queenside_castling_is_free(index, board, turn);

    if (kingside_free && !m_castling->kingside_canceled)
    {
        m_castling->kingside_special_moves = {index + 2, index + 1};
        legal_moves().push_back(index + 2);
    }
    if (queenside_free && !m_castling->queenside_canceled)
    {
        m_castling->queenside_special_moves = {index - 2, index - 1};
        legal_moves().push_back(index - 2);
    }
}

bool King::kingside_castling_is_free(const int index, const Chessboard& board, Turn& turn)
{
    for (int i{index + 1}; i < m_castling->kingside_rook_index; i++)
    {
        if (!cell_is_free(i, board, turn))
            return false;
    }
    return true;
}

bool King::queenside_castling_is_free(const int index, const Chessboard& board, Turn& turn)
{
    for (int i{index - 1}; i > m_castling->queenside_rook_index; i--)
    {
        if (!cell_is_free(i, board, turn))
            return false;
    }
    return true;
}

bool King::cell_is_free(const int i, const Chessboard& board, Turn& turn)
{
    if (board[i])
        return false;

    std::unordered_map<Type, std::vector<int>> threats;
    add_threatening_pieces(i, turn, threats, board);
    return threats.empty();
}