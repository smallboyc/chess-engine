#pragma once
#include <glfw/src/internal.h>
#include <imgui.h>
#include <memory>
#include <optional>
#include "./gui/Settings.hpp"
#include "Animation.hpp"
#include "King.hpp"
#include "Piece.hpp"
#include "Types.hpp"
#include "utils.hpp"

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

class ChessGame {
public:
    ChessGame()
    {
        initialize_board();
        set_current_king();
    };
    void load_all_textures(const std::string& folder_path);
    void initialize_board();
    // PLAY
    void play(Settings& settings, Animation& animation);
    //
    Chessboard&                    get_chessboard() { return m_board; };
    Turn                           get_turn() const { return m_turn; };
    Textures                       get_textures() const { return m_textures; };
    std::optional<Texture>         get_selected_piece_texture();
    std::optional<int>             get_selected_piece_index();
    std::vector<MoveStatus>        get_moves_saved() const { return m_move_saves; };    // used for the history in the GUI
    std::optional<MoveProcessing>& get_move_processing() { return m_move_processing; }; // used for smooth 3D animation
    Warnings                       get_warnings() { return m_warnings; };
    // listeners : used to update board size & colors in the GUI
    void board_size_listener(Settings& settings);
    void board_colors_listener(Settings& settings);

private:
    //* VARS
    float                                 m_size{};
    ColorCells                            m_color_cells;
    Chessboard                            m_board; // CHESSBOARD
    std::optional<std::pair<int, Piece*>> m_selected_piece{std::nullopt};
    std::pair<int, King*>                 m_current_king;
    Turn                                  m_turn;
    Status                                m_status;
    Warnings                              m_warnings;
    Textures                              m_textures;
    std::vector<MoveStatus>               m_move_saves;                    // for the gui
    std::optional<MoveProcessing>         m_move_processing{std::nullopt}; // from & to (for 3D renderer animation)

    //* METHODS
    // init
    void                   set_piece_on_board(const Type& piece_type, const std::vector<int>& piece_positions, const Color& piece_color);
    std::unique_ptr<Piece> create_piece(const Type& piece_type, const Color& piece_color);
    std::optional<int>     draw_cell(int cell_index, const ImVec4& color);
    // moves
    bool player_move(int selected_cell_index);
    bool is_a_dangerous_move(int selected_cell_index);
    void cancel_player_move(int from, int to, std::unique_ptr<Piece> respawn_enemy);
    void clear_all_legal_moves();
    // scopes
    void display_scopes(int selected_cell_index, Settings& settings);
    // selections
    bool piece_can_be_selected(int cell_index);
    void select_piece(int selected_cell_index);
    bool piece_selected() const;
    void toggle_king_selection();
    void toggle_defender_selection(int index, const std::vector<int>& moves);
    void switch_player_turn();
    // current king
    void set_current_king();
    void bind_rooks_with_king();
    // handlers for game situations
    void handle_free_play(int clicked_cell);
    void handle_check_situation(int clicked_cell);
    void handle_double_check();
    void handle_single_check();
    // pawn transform
    void show_all_transform_options();
};