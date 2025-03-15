#include "./game2D/ChessGame.hpp"
#include "./renderer3D/Renderer3D.hpp"
#include "Animation.hpp"
#include "game2D/gui/GameTracker.hpp"

int main()
{
    // Game 2D
    ChessGame   chess2D;
    GameTracker game_tracker;
    Settings    settings;
    // Renderer 3D
    Renderer3D renderer3D(1280, 720);
    //
    Animation animation;
    //
    quick_imgui::loop(
        "Chess App",
        {
            .init = [&]() { 
            ImGui::GetStyle().ItemSpacing = ImVec2(0.0f, 0.0f); 
            // load_font("../../assets/fonts/PoetsenOne-Regular.ttf", 20.0f);
             //GAME 2D
            chess2D.load_all_textures("../../assets/images/");
            //RENDER 3D
            renderer3D.init(chess2D.get_chessboard()); },
            .loop = [&]() { 
                //GAME 2D
                ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX));
                ImGui::Begin("Chessboard", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
                chess2D.board_size_listener(settings);
                chess2D.board_colors_listener(settings);
                chess2D.play(settings, animation);
                ImGui::End();
                //RENDER 3D
                renderer3D.run(chess2D.get_chessboard(), chess2D.get_move_processing(), animation);
            // user windows
            settings.show();
            game_tracker.show(chess2D); },
            // CALLBACKS
            .key_callback             = [&](int key, int /*scancode*/, int action, int mods) { 
            renderer3D.useCamera().free_move_callback(key, action);
            renderer3D.toggle_active_camera_callback(key, action); },
            .mouse_button_callback    = [&](int button, int action, int mods) {},
            .cursor_position_callback = [&](double xpos, double ypos) { renderer3D.useCamera().track_ball_move_callback(xpos, ypos); },
            .scroll_callback          = [&](double /*xoffset*/, double yoffset) { renderer3D.useCamera().zoom_callback(yoffset); },
            .window_size_callback     = [&](int width, int height) { renderer3D.window_size_callback(width, height); },
        }
    );
    return 0;
}
