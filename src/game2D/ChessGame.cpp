#include "ChessGame.hpp"
#include <imgui.h>
#include <cmath>
#include <filesystem>
#include <memory>
#include <optional>
#include <unordered_map>
#include "Bishop.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Pawn.hpp"
#include "Piece.hpp"
#include "Queen.hpp"
#include "Rook.hpp"
#include "Types.hpp"
#include "utils.hpp"

// Settings
void ChessGame::board_size_listener(Settings& settings)
{
    m_size = settings.get_board_size();
}
void ChessGame::board_colors_listener(Settings& settings)
{
    m_color_cells = {settings.get_primary_color(), settings.get_secondary_color()};
}

// Get texture of the selected piece for the GUI
std::optional<Texture> ChessGame::get_selected_piece_texture()
{
    if (m_selected_piece.has_value())
    {
        auto& [index, piece] = m_selected_piece.value();
        return piece->get_texture(m_textures);
    }
    return std::nullopt;
}

// Get index of the selected piece for the camera
std::optional<int> ChessGame::get_selected_piece_index()
{
    if (m_selected_piece.has_value())
    {
        auto& [index, piece] = m_selected_piece.value();
        return index;
    }
    return std::nullopt;
}

// Initialize the chessboard by placing the pieces in their starting positions
void ChessGame::initialize_board()
{
    std::array<PiecePositions, 6> initial_positions = {
        PiecePositions{Type::Pawn, {8, 9, 10, 11, 12, 13, 14, 15}, {48, 49, 50, 51, 52, 53, 54, 55}},
        PiecePositions{Type::Rook, {0, 7}, {56, 63}},
        PiecePositions{Type::Knight, {1, 6}, {57, 62}},
        PiecePositions{Type::Bishop, {2, 5}, {58, 61}},
        PiecePositions{Type::Queen, {3}, {59}},
        PiecePositions{Type::King, {4}, {60}}
    };

    for (const PiecePositions& piece_positions : initial_positions)
    {
        set_piece_on_board(piece_positions.piece_type, piece_positions.white_position, Color::White);
        set_piece_on_board(piece_positions.piece_type, piece_positions.black_position, Color::Black);
    }
}

// Set the pieces on the chessboard
void ChessGame::set_piece_on_board(const Type& piece_type, const std::vector<int>& piece_positions, const Color& piece_color)
{
    for (const int& piece_position : piece_positions)
        m_board[piece_position] = create_piece(piece_type, piece_color);
}

// Create a piece of the specified type and color
std::unique_ptr<Piece> ChessGame::create_piece(const Type& piece_type, const Color& piece_color)
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

// Set the current king based on the player's turn
void ChessGame::set_current_king()
{
    for (int i{0}; i < m_board.size(); i++)
    {
        if (m_board[i] && m_board[i]->get_type() == Type::King && m_board[i]->get_color() == m_turn.current_player)
        {
            m_current_king = {i, dynamic_cast<King*>(m_board[i].get())};
            // bind rooks with king only at the beginning of the game (1st & 2nd turn)
            if (m_turn.total < 2)
            {
                bind_rooks_with_king();
            }
            break;
        }
    }
}

// Bind the rooks with the king for castling
void ChessGame::bind_rooks_with_king()
{
    std::vector<int> rooks;
    for (int j{0}; j < m_board.size(); j++)
    {
        if (m_board[j] && m_board[j]->get_type() == Type::Rook && m_board[j]->get_color() == m_turn.current_player)
        {
            rooks.push_back(j);
        }
    }
    if (!rooks.empty())
        m_current_king.second->set_castling(rooks);
}

// Draw a cell on the chessboard
std::optional<int> ChessGame::draw_cell(int cell_index, const ImVec4& color)
{
    ImGui::PushID(cell_index);
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    Texture cell_texture = is_empty_cell(cell_index, m_board) ? m_textures["null.png"] : m_board[cell_index]->get_texture(m_textures);
    bool    clicked      = ImGui::ImageButton(reinterpret_cast<ImTextureID>(cell_texture), {m_size, m_size});
    ImGui::PopStyleColor();
    ImGui::PopID();

    if ((cell_index + 1) % static_cast<int>(std::sqrt(m_board.size())) != 0)
        ImGui::SameLine(0.0f, 0.0f);

    return clicked ? std::optional<int>{cell_index} : std::nullopt;
}

// Play the game (main loop for the 2D chess game)
void ChessGame::play(Settings& settings, Animation& animation)
{
    ImVec4 current_color_cell = m_color_cells.first;
    for (int i{0}; i < m_board.size(); i++)
    {
        if (i % 8 != 0)
            current_color_cell = equal_cells(current_color_cell, m_color_cells.first) ? m_color_cells.second : m_color_cells.first;

        std::optional<int> clicked_cell = draw_cell(i, current_color_cell);

        if (clicked_cell.has_value() && !animation.is_animating && !m_turn.pawn_promotion.has_value())
        {
            if (m_status.free_play)
            {
                handle_free_play(*clicked_cell);
            }
            else
            {
                handle_check_situation(*clicked_cell);
            }
        }
        display_scopes(i, settings);
    }

    if (m_turn.pawn_promotion.has_value())
    {
        show_all_transform_options();
    }
}

// Free play is the classic chess game function without any special rules (check, checkmate)
void ChessGame::handle_free_play(int clicked_cell)
{
    if (piece_can_be_selected(clicked_cell))
    {
        if (piece_selected())
        {
            m_selected_piece->second->legal_moves().clear();
        }
        select_piece(clicked_cell);
    }
    if (piece_selected() && player_move(clicked_cell))
    {
        if (!m_turn.pawn_promotion.has_value())
        {
            m_move_saves.emplace_back(clicked_cell, m_board[clicked_cell]->get_texture(m_textures), m_selected_piece->second->has_captured());
            switch_player_turn();
        }
    }
    //! current_player
    auto& [index, king] = m_current_king;

    if (king->is_in_check(index, m_turn, m_board))
    {
        m_status.free_play = false;
        m_warnings.check   = true;
        if (king->is_double_check(index, m_turn, m_board))
        {
            handle_double_check();
        }
        else
        {
            handle_single_check();
        }
    }
    else if (king->get_castling().has_value())
    {
        king->check_for_castling(index, m_board, m_turn);
    }
}

// Check situation is a part of the chess game where the king is in check or checkmate
void ChessGame::handle_check_situation(int clicked_cell)
{
    auto& [index, king] = m_current_king;
    //
    if (m_status.waiting_for_king_move)
    {
        if (clicked_cell == index)
        {
            toggle_king_selection();
        }

        if (m_status.king_is_selected && player_move(clicked_cell))
        {
            m_move_saves.emplace_back(clicked_cell, m_board[clicked_cell]->get_texture(m_textures), m_selected_piece->second->has_captured());
            m_status.reset();
            switch_player_turn();
        }
    }
    if (m_status.waiting_for_defender_move)
    {
        for (const auto& [index, moves] : king->get_defenders())
        {
            if (clicked_cell == index)
            {
                toggle_defender_selection(index, moves);
            }
        }
        if (m_status.defender_is_selected && player_move(clicked_cell))
        {
            m_move_saves.emplace_back(clicked_cell, m_board[clicked_cell]->get_texture(m_textures), m_selected_piece->second->has_captured());
            m_status.reset();
            switch_player_turn();
        }
    }
}

// Active the check situation when only the king must move to survive.
void ChessGame::handle_double_check()
{
    auto& [index, king] = m_current_king;
    //
    m_status.waiting_for_king_move = king->can_escape(index, m_turn, m_board);
    if (!m_status.waiting_for_king_move)
        m_warnings.checkmate = true;
}

// Active the check situation when the king can move or a defender can move to protect the king.
void ChessGame::handle_single_check()
{
    auto& [index, king] = m_current_king;
    //
    m_status.waiting_for_defender_move = king->has_allies_to_defend(m_turn, m_board);
    m_status.waiting_for_king_move     = king->can_escape(index, m_turn, m_board);

    if (!m_status.waiting_for_defender_move && !m_status.waiting_for_king_move)
        m_warnings.checkmate = true;
}

// Toggle the selection of the king (first click = selected, second click = deselected)
void ChessGame::toggle_king_selection()
{
    if (m_status.king_is_selected) // cancel selection
    {
        m_status.king_is_selected = false;
        m_selected_piece.reset();
    }
    else // active selection
    {
        auto& [index, king]           = m_current_king;
        m_selected_piece              = {index, king};
        auto& [_, piece]              = m_selected_piece.value();
        piece->legal_moves()          = king->get_escape_moves();
        m_status.king_is_selected     = true;
        m_status.defender_is_selected = false;
    }
}

// Toggle the selection of a defender (first click = selected, second click = deselected)
void ChessGame::toggle_defender_selection(const int index, const std::vector<int>& moves)
{
    if (m_status.defender_is_selected) // cancel selection
    {
        m_status.defender_is_selected = false;
        m_selected_piece.reset();
    }
    else // active selection
    {
        m_selected_piece              = {index, m_board[index].get()};
        auto& [index, piece]          = m_selected_piece.value();
        piece->legal_moves()          = moves;
        m_status.defender_is_selected = true;
        m_status.king_is_selected     = false;
    }
}

void ChessGame::select_piece(int selected_cell_index)
{
    m_selected_piece     = {selected_cell_index, m_board[selected_cell_index].get()};
    auto& [index, piece] = m_selected_piece.value();
    piece->set_legal_moves(index, m_board, m_turn);
}

// Move the piece to the selected cell if the move is legal + check if the king is in check after the move
bool ChessGame::player_move(int selected_cell_index)
{
    auto& [p_index, piece] = m_selected_piece.value();
    auto& [k_index, king]  = m_current_king;

    if (piece->player_move_is_legal(selected_cell_index))
    {
        // We have to keep information about the enemy piece that will be deleted (if the move is canceled).
        auto deleted_enemy = std::move(m_board[selected_cell_index]);

        piece->move_piece(p_index, selected_cell_index, m_board, m_turn, m_move_processing);

        // Check if the move is legal (the king is not in check after the move) else cancel the move.
        if (piece)
        {
            if (piece->get_type() != Type::King && king->is_in_check(k_index, m_turn, m_board))
            {
                m_warnings.dangerous_move = true;
                cancel_player_move(selected_cell_index, p_index, std::move(deleted_enemy));
                return false;
            }
            else
            {
                piece->set_capture(static_cast<bool>(deleted_enemy));
            }
        }

        clear_all_legal_moves();
        king->reset_buffers();
        return true;
    }
    return false;
}

// Cancel the player move if the move is illegal (the king is in check after the move)
void ChessGame::cancel_player_move(int from, int to, std::unique_ptr<Piece> respawn_enemy)
{
    m_board[to]   = std::move(m_board[from]);
    m_board[from] = std::move(respawn_enemy);
    m_turn.total--;
    m_move_processing.reset();
}

// Display the scopes of the selected piece and the king (scopes are colored cells that show the possible moves of a piece)
void ChessGame::display_scopes(int cell_index, Settings& settings)
{
    auto& [index, king] = m_current_king;
    if (piece_selected() && settings.is_legal_moves_scope_active())
    {
        m_selected_piece->second->draw_scopes(cell_index, m_turn, m_board);
    }
    if (king->is_in_check(index, m_turn, m_board))
    {
        if (settings.is_highlight_threats_scope_active())
            king->draw_threat_scopes(index, cell_index);
        king->draw_defenders_scopes(m_selected_piece, cell_index);
    }
}

// Check if a piece can be selected (meaning it's not empty an empty cell and belongs to the current player)
bool ChessGame::piece_can_be_selected(int cell_index)
{
    return !is_empty_cell(cell_index, m_board) && m_board[cell_index]->get_color() == m_turn.current_player;
}

// Check if a piece is selected
bool ChessGame::piece_selected() const
{
    return m_selected_piece.has_value();
}

// Switch the player's turn, reset the selected piece and warnings & set the new current king
void ChessGame::switch_player_turn()
{
    m_selected_piece.reset();
    m_warnings.reset();
    m_turn.current_player = m_turn.current_player == Color::White ? Color::Black : Color::White;
    set_current_king();
}

// Load all 2D textures of the pieces.
void ChessGame::load_all_textures(const std::string& folder_path)
{
    for (const auto& entry : std::filesystem::directory_iterator(folder_path))
    {
        if (entry.is_regular_file())
        {
            std::string extension = entry.path().extension().string();
            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
            {
                std::string filename  = entry.path().filename().string();
                std::string full_path = entry.path().string();
                m_textures[filename]  = load_texture_from_file(full_path);
            }
        }
    }
}

// Clear all legal moves for all pieces on the board after a move
void ChessGame::clear_all_legal_moves()
{
    for (int i{0}; i < m_board.size(); i++)
    {
        if (m_board[i] && !m_board[i]->legal_moves().empty())
        {
            m_board[i]->legal_moves().clear();
        }
    }
}

// Show all transformation options for the pawn promotion in the GUI
void ChessGame::show_all_transform_options()
{
    ImGui::Begin("Transform option");

    std::unordered_map<Type, std::string> piece_names = {
        {Type::Bishop, "b"},
        {Type::Knight, "n"},
        {Type::Rook, "r"},
        {Type::Queen, "q"}
    };

    for (auto& [type, piece_name] : piece_names)
    {
        auto texture_path = m_textures[(m_turn.current_player == Color::White ? "w" : "b") + piece_name + ".png"];
        if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture_path), {m_size, m_size}))
        {
            m_board[m_turn.pawn_promotion->to] = create_piece(type, m_turn.current_player);
            m_turn.pawn_promotion.reset();
            m_move_processing = {m_turn.pawn_promotion->from, m_turn.pawn_promotion->to};
            m_move_saves.emplace_back(m_turn.pawn_promotion->to, texture_path, false);
            switch_player_turn();
        }
        ImGui::SameLine();
    }

    ImGui::End();
}
