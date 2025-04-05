#include "App.hpp"
#include "game2D/utils.hpp"

void App::init()
{
    ImGui::GetStyle().ItemSpacing = ImVec2(0.0f, 0.0f);
    // FONT
    load_font("../../assets/fonts/PoetsenOne-Regular.ttf", 16.0f);
    // GAME 2D
    m_chess2D.load_all_textures("../../assets/images/game2D/");
    // RENDER 3D
    m_renderer3D.init(m_chess2D.get_chessboard());
}

void App::run()
{
    // GAME 2D LISTENERS
    m_chess2D.board_size_listener(m_settings);
    m_chess2D.board_colors_listener(m_settings);
    // GAME 2D WINDOW
    m_game2D_wrapper.wrap(m_chess2D, m_settings, animation, "ChessGame 2D");
    // RENDER 3D
    m_renderer3D.camera_position_and_orientation_listener(m_chess2D.get_selected_piece_index(), m_chess2D.get_turn().current_player);
    m_renderer3D.run(m_chess2D.get_chessboard(), m_chess2D.get_move_processing(), animation, m_settings);
    // SETTINGS WINDOW
    m_settings.show(animation, m_chess2D.get_selected_piece_index(), m_renderer3D.use_camera());
    // GAME TRACKER WINDOW
    m_game_tracker.show(m_chess2D);
}

// callbacks
void App::key_callback(int key, int action)
{
    m_renderer3D.use_camera().free_move_callback(key, action);
    m_renderer3D.toggle_active_camera_callback(key, action);
}
//
void App::mouse_button_callback(int button, int action, int mods) {}
//
void App::cursor_position_callback(double xpos, double ypos)
{
    m_renderer3D.use_camera().track_ball_move_callback(xpos, ypos);
}
void App::scroll_callback(double yoffset)
{
    m_renderer3D.use_camera().zoom_callback(yoffset);
}
void App::window_size_callback(int width, int height)
{
    m_renderer3D.window_size_callback(width, height);
}