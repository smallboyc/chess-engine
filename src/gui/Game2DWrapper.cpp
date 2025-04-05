#include "Game2DWrapper.hpp"

void Game2DWrapper::wrap(ChessGame& chess_game, Settings& settings, Animation& animation, const char* wrapper_name)
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin(wrapper_name, nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    chess_game.play(settings, animation);
    ImGui::End();
}