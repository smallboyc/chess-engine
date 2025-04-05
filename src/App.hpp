#pragma once
#include "./game2D/ChessGame.hpp"
#include "./renderer3D/Renderer3D.hpp"
#include "./renderer3D/Animation.hpp"
#include "gui/Game2DWrapper.hpp"
#include "gui/GameTracker.hpp"

class App {
public:
    explicit App(const char* app_name)
        : m_app_name(app_name){};
    ~App() = default;

    App(const App&)                = delete;
    App& operator=(const App&)     = delete;
    App(App&&) noexcept            = delete;
    App& operator=(App&&) noexcept = delete;

    const char* get_app_name() const { return m_app_name; }
    //
    void init();
    void run();

    // callbacks
    void key_callback(int key, int action);
    void mouse_button_callback(int button, int action, int mods);
    void cursor_position_callback(double xpos, double ypos);
    void scroll_callback(double yoffset);
    void window_size_callback(int width, int height);

private:
    const char* m_app_name;
    ChessGame   m_chess2D;
    Renderer3D  m_renderer3D{1280, 720};
    //
    Game2DWrapper m_game2D_wrapper;
    GameTracker m_game_tracker;
    Settings    m_settings;
    //
    Animation animation;
};