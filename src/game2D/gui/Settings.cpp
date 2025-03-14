#include "Settings.hpp"
#include <imgui.h>

void Settings::show()
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
    ImGui::SliderFloat("##SizeSlider", &m_board_size, 30.0f, 70.0f, "%.0f");
}