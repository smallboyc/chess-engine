#pragma once
#include <imgui.h>

class Settings {
public:
    Settings() = default;
    void show();
    //
    bool is_legal_moves_scope_active() const { return m_show_legal_moves_scope; };
    bool is_highlight_threats_scope_active() const { return m_highlight_threats_scope; };
    //
    ImVec4 get_primary_color() const { return m_primary_color; };
    ImVec4 get_secondary_color() const { return m_secondary_color; };
    float  get_board_size() const { return m_board_size; };

private:
    void color_picker_widget();
    void board_size_slider_widget();
    //
    bool   m_show_legal_moves_scope{true};
    bool   m_highlight_threats_scope{true};
    ImVec4 m_primary_color{0.18f, 0.322f, 0.263f, 1.0f};
    ImVec4 m_secondary_color{0.518f, 0.741f, 0.647f, 1.0f};
    float  m_board_size{50.f};
};