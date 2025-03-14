#include <iostream>
#include "Renderer3D.hpp"

int main()
{
    Renderer3D renderer3D(1280, 720);
    quick_imgui::loop(
        "Chess App",
        {
            .init                     = [&]() { renderer3D.init(); },
            .loop                     = [&]() { renderer3D.run(); },
            .key_callback             = [&](int key, int /*scancode*/, int action, int mods) { 
            renderer3D.useCamera().free_move_callback(key, action); 
            renderer3D.delete_piece_callback(key, action); },
            .mouse_button_callback    = [&](int button, int action, int mods) {},
            .cursor_position_callback = [&](double xpos, double ypos) { renderer3D.useCamera().track_ball_move_callback(xpos, ypos); },
            .scroll_callback          = [&](double /*xoffset*/, double yoffset) { renderer3D.useCamera().zoom_callback(yoffset); },
            .window_size_callback     = [&](int width, int height) { renderer3D.window_size_callback(width, height); },
        }
    );
    return 0;
}
