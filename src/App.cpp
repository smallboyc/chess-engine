#include "./game2D/Chessboard.hpp"
#include "Renderer3D.hpp"
#include "game2D/gui/GameTracker.hpp"

int main()
{
    Chessboard  game2D;
    GameTracker game_tracker;
    Settings    settings;
    Renderer3D  renderer3D(1280, 720);
    quick_imgui::loop(
        "Chess App",
        {
            .init = [&]() { 
            // load_font("../../assets/fonts/PoetsenOne-Regular.ttf", 20.0f);
            game2D.load_all_textures("../../assets/images/");
            renderer3D.init(game2D.get_chessboard());
            ImGui::GetStyle().ItemSpacing = ImVec2(0.0f, 0.0f); },
            .loop = [&]() { 
                //RENDER 3D
                renderer3D.run(game2D.get_chessboard());
                //GAME 2D
                ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX));
                // chessboard window
                ImGui::Begin("Chessboard", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
                game2D.board_size_listener(settings);
                game2D.board_colors_listener(settings);
                game2D.play(settings);
                ImGui::End();
            // user windows
            settings.show();
            game_tracker.show(game2D); },
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
