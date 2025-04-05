#include <quick_imgui/quick_imgui.hpp>
#include "App.hpp"

int main()
{
    App chess_app("Chess App");
    quick_imgui::loop(
        chess_app.get_app_name(),
        {
            .init = [&]() { chess_app.init(); },
            .loop = [&]() { chess_app.run(); },
            // CALLBACKS
            .key_callback             = [&](int key, int /*scancode*/, int action, int mods) { chess_app.key_callback(key, action); },
            .mouse_button_callback    = [&](int button, int action, int mods) {},
            .cursor_position_callback = [&](double xpos, double ypos) { chess_app.cursor_position_callback(xpos, ypos); },
            .scroll_callback          = [&](double /*xoffset*/, double yoffset) { chess_app.scroll_callback(yoffset); },
            .window_size_callback     = [&](int width, int height) { chess_app.window_size_callback(width, height); },
        }
    );
}
