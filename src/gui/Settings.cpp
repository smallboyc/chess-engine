#include "Settings.hpp"
#include <imgui.h>
#include <cstdarg>
#include "Animation.hpp"

void Settings::show(Animation& animation, std::optional<int> selected_piece, glmax::Camera& camera)
{
    ImGui::Begin("Settings");
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    ImGui::Checkbox("Show Legal Moves", &m_show_legal_moves_scope);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Checkbox("Highlight Threats", &m_highlight_threats_scope);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    color_picker_widget();
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    board_size_slider_widget();
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    animation_duration_widget(animation);
    //
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    if (camera.is_locked())
    {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.4f, 1.0f), "Press L to unlock the camera rotation");
    }
    else
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Press L to lock the camera rotation");
    }

    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    if (selected_piece.has_value())
    {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "By unchecking this box, the camera will follow the selected piece");
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Checkbox("Active Trackball", &camera.is_track_ball());
    }

    ImGui::End();
}

void Settings::color_picker_widget()
{
    ImGui::Text("Chessboard colors:");
    ImGui::Dummy(ImVec2(20.0f, 5.0f));
    ImGui::ColorEdit4("##Color0", reinterpret_cast<float*>(&m_primary_color), ImGuiColorEditFlags_NoInputs);
    ImGui::SameLine(0.0f, 5.0f);
    ImGui::ColorEdit4("##Color1", reinterpret_cast<float*>(&m_secondary_color), ImGuiColorEditFlags_NoInputs);
}

void Settings::board_size_slider_widget()
{
    ImGui::Text("Chessboard size:");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::SliderFloat("##SizeSlider0", &m_board_size, 30.0f, 70.0f, "%.0f");
}

void Settings::animation_duration_widget(Animation& animation)
{
    ImGui::Text("Animation duration:");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::SliderFloat("##SizeSlider1", &animation.animation_duration, 0.0f, 2.0f, "%.0f");
}