// #include <glad/glad.h>
// #include <imgui.h>
// #include <quick_imgui/quick_imgui.hpp>
// #include <string>
// #include "./gui/GameTracker.hpp"
// #include "./gui/Settings.hpp"
// #include "Chessboard.hpp"
// #include "utils.hpp"

// int main()
// {
//     // TODO(smallboyc): faire une nouvelle classe ChessGame, puis Chessboard ne gère que l'affichage du board
//     Chessboard  chessboard;
//     GameTracker game_tracker;
//     Settings    settings;

//     quick_imgui::loop(
//         "Chess",
//         [&]() {
//             load_font("../../assets/fonts/PoetsenOne-Regular.ttf", 20.0f);
//             chessboard.load_all_textures("../../assets/images/");
//             ImGui::GetStyle().ItemSpacing = ImVec2(0.0f, 0.0f);
//         },
//         [&]() {
//             ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX));
//             // chessboard window
//             ImGui::Begin("Chessboard", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
//             chessboard.board_size_listener(settings);
//             chessboard.board_colors_listener(settings);
//             chessboard.play(settings);
//             ImGui::End();

//             // user windows
//             settings.show(chessboard);
//             game_tracker.show(chessboard);
//         }
//     );
// }