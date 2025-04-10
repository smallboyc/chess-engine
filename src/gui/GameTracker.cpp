#include "GameTracker.hpp"
#include <format>

void GameTracker::show(ChessGame& chess_game)
{
    std::vector<MoveStatus> all_moves = chess_game.get_moves_saved();

    ImGui::Begin("Tracker");

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("%s", std::format("Total moves : {}", all_moves.size()).c_str());
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    history_moves_widget(chess_game, all_moves);

    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::Text("%s", std::format("{} turn's !", player_turn(chess_game.get_turn().current_player)).c_str());
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    warnings_widget(chess_game);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    selected_piece_widget(chess_game);

    ImGui::End();
}

void GameTracker::history_moves_widget(ChessGame& chess_game, std::vector<MoveStatus>& all_moves)
{
    ImGui::BeginChild("ScrollHistoryMoves", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (size_t i{0}; i < all_moves.size(); i++)
    {
        auto& [move, texture, capture]              = all_moves[i];
        std::pair<char, char> board_coordinate_move = get_board_coordinate(get_position(move));

        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::Text("%s", std::format("{} : ", i + 1).c_str());
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::Image(reinterpret_cast<ImTextureID>(texture), ImVec2(27.0f, 27.0f));
        ImGui::SameLine(0.0f, 0.0f);
        if (capture)
            ImGui::Image(reinterpret_cast<ImTextureID>(chess_game.get_textures()["target.png"]), ImVec2(17.0f, 17.0f));
        ImGui::SameLine(0.0f, 5.0f);
        ImGui::Text("%s", std::format("{}{}", board_coordinate_move.first, board_coordinate_move.second).c_str());

        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::Separator();
    }

    ImGui::EndChild();
}

void GameTracker::selected_piece_widget(ChessGame& chess_game)
{
    float text_height = ImGui::GetTextLineHeight();
    float image_size  = 35.0f;
    float offset_y    = (text_height - image_size) * 0.5f;

    ImGui::Text("Selected piece : ");
    ImGui::SameLine(0.0f, 5.0f);

    if (chess_game.get_selected_piece_texture().has_value())
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset_y);
        ImGui::ImageButton(
            reinterpret_cast<ImTextureID>(chess_game.get_selected_piece_texture().value()),
            ImVec2(image_size, image_size)
        );
    }
}

void GameTracker::warnings_widget(ChessGame& chess_game)
{
    std::optional<std::string> warning;

    if (chess_game.get_warnings().checkmate)
    {
        warning = "Checkmate !";
    }
    else if (chess_game.get_warnings().check)
    {
        warning = "Check !";
    }
    else if (chess_game.get_warnings().dangerous_move)
    {
        warning = "Forbidden move !";
    }
    else
    {
        if (warning.has_value())
            warning.reset();
    }

    if (warning.has_value())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Warning : %s", warning.value().c_str());
    }
}