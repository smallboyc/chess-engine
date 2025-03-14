#include "Chessboard.hpp"
#include <imgui.h>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include "Bishop.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Pawn.hpp"
#include "Piece.hpp"
#include "Queen.hpp"
#include "Rook.hpp"
#include "Types.hpp"
#include "utils.hpp"

void Chessboard::board_size_listener(Settings& settings)
{
    m_size = settings.get_board_size();
}
void Chessboard::board_colors_listener(Settings& settings)
{
    m_color_cells = {settings.get_primary_color(), settings.get_secondary_color()};
}

std::optional<Texture> Chessboard::get_selected_piece_texture()
{
    if (m_selected_piece.has_value())
    {
        auto& [index, piece] = m_selected_piece.value();
        return piece->get_texture(m_textures);
    }
    return std::nullopt;
}

// Create
void Chessboard::initialize_board()
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

void Chessboard::set_current_king()
{
    for (int i{0}; i < m_board.size(); i++)
    {
        if (m_board[i] && m_board[i]->get_type() == Type::King && m_board[i]->get_color() == m_turn.current_player)
        {
            m_current_king = {i, dynamic_cast<King*>(m_board[i].get())};
            // on set les tours uniquement aux 2 premiers tours.
            if (m_turn.total < 2)
            {
                bind_rooks_with_king();
            }
            break;
        }
    }
}

void Chessboard::bind_rooks_with_king()
{
    std::vector<int> rooks;
    // on lui associe les ses tours
    for (int j{0}; j < m_board.size(); j++)
    {
        if (m_board[j] && m_board[j]->get_type() == Type::Rook && m_board[j]->get_color() == m_turn.current_player)
        {
            rooks.push_back(j);
        }
    }
    m_current_king.second->set_castling(rooks);
}

std::optional<int> Chessboard::draw_cell(int cell_index, const ImVec4& color)
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

void Chessboard::play(Settings& settings)
{
    ImVec4 current_color_cell = m_color_cells.first;
    for (int i{0}; i < m_board.size(); i++)
    {
        if (i % 8 != 0)
            current_color_cell = are_equal(current_color_cell, m_color_cells.first) ? m_color_cells.second : m_color_cells.first;

        std::optional<int> clicked_cell = draw_cell(i, current_color_cell);

        if (clicked_cell.has_value())
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
}

void Chessboard::handle_free_play(int clicked_cell)
{
    // current_player
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
        m_move_saves.emplace_back(clicked_cell, m_board[clicked_cell]->get_texture(m_textures), m_selected_piece->second->has_captured());
        switch_player_turn();
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

void Chessboard::handle_check_situation(int clicked_cell)
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

void Chessboard::handle_double_check()
{
    auto& [index, king] = m_current_king;
    //
    m_status.waiting_for_king_move = king->can_escape(index, m_turn, m_board);
    if (!m_status.waiting_for_king_move)
        m_warnings.checkmate = true;
}

void Chessboard::handle_single_check()
{
    auto& [index, king] = m_current_king;
    //
    m_status.waiting_for_defender_move = king->has_allies_to_defend(m_turn, m_board);
    m_status.waiting_for_king_move     = king->can_escape(index, m_turn, m_board);

    if (!m_status.waiting_for_defender_move && !m_status.waiting_for_king_move)
        m_warnings.checkmate = true;
}

void Chessboard::toggle_king_selection()
{
    //
    if (m_status.king_is_selected)
    {
        m_status.king_is_selected = false;
        m_selected_piece.reset();
        std::cout << "Roi déselectionné" << "\n";
    }
    else
    {
        auto& [index, king]           = m_current_king;
        m_selected_piece              = {index, king};
        auto& [_, piece]              = m_selected_piece.value();
        piece->legal_moves()          = king->get_escape_moves();
        m_status.king_is_selected     = true;
        m_status.defender_is_selected = false;
        std::cout << "Roi sélectionné" << "\n";
    }
}

void Chessboard::toggle_defender_selection(const int index, const std::vector<int>& moves)
{
    if (m_status.defender_is_selected)
    {
        m_status.defender_is_selected = false;
        m_selected_piece.reset();
        std::cout << "Allié déselectionné" << "\n";
    }
    else
    {
        m_selected_piece              = {index, m_board[index].get()};
        auto& [index, piece]          = m_selected_piece.value();
        piece->legal_moves()          = moves;
        m_status.defender_is_selected = true;
        m_status.king_is_selected     = false;
        std::cout << "Allié sélectionné" << "\n";
    }
}

void Chessboard::select_piece(int selected_cell_index)
{
    m_selected_piece     = {selected_cell_index, m_board[selected_cell_index].get()};
    auto& [index, piece] = m_selected_piece.value();
    piece->set_legal_moves(index, m_board, m_turn);
}

bool Chessboard::player_move(int selected_cell_index)
{
    auto& [p_index, piece] = m_selected_piece.value();
    auto& [k_index, king]  = m_current_king;

    if (piece->player_move_is_legal(selected_cell_index))
    {
        // Sauvegarder l'état actuel pour annuler le mouvement si nécessaire
        auto deleted_enemy = std::move(m_board[selected_cell_index]);

        // Effectuer le mouvement
        piece->move_piece(p_index, selected_cell_index, m_board, m_turn);

        // Vérifier si le roi est en échec après le mouvement que si c'est pas le roi qui bouge (car le roi a pas à se protéger lui même).
        if (piece->get_type() != Type::King && king->is_in_check(k_index, m_turn, m_board))
        {
            m_warnings.dangerous_move = true;
            // Annuler le mouvement
            cancel_player_move(selected_cell_index, p_index, std::move(deleted_enemy));
            return false;
        }
        else
        {
            piece->set_capture(static_cast<bool>(deleted_enemy));
        }

        // Réinitialiser tous les "buffers".
        clear_all_legal_moves();
        king->reset_buffers();

        return true;
    }
    return false;
}

void Chessboard::cancel_player_move(int from, int to, std::unique_ptr<Piece> respawn_enemy)
{
    m_board[to]   = std::move(m_board[from]);
    m_board[from] = std::move(respawn_enemy);
    m_turn.total--;
}

void Chessboard::display_scopes(int cell_index, Settings& settings)
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
bool Chessboard::piece_can_be_selected(int cell_index)
{
    return !is_empty_cell(cell_index, m_board) && m_board[cell_index]->get_color() == m_turn.current_player;
}

bool Chessboard::piece_selected() const
{
    return m_selected_piece.has_value();
}

void Chessboard::switch_player_turn()
{
    m_selected_piece.reset();
    m_warnings.reset();
    m_turn.current_player = m_turn.current_player == Color::White ? Color::Black : Color::White;
    set_current_king();
}

// textures
void Chessboard::load_all_textures(const std::string& folder_path)
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

// utils
void Chessboard::clear_all_legal_moves()
{
    for (int i{0}; i < m_board.size(); i++)
    {
        if (m_board[i] && !m_board[i]->legal_moves().empty())
        {
            m_board[i]->legal_moves().clear();
        }
    }
}