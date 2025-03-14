#include "utils.hpp"
#include <glad/glad.h>
#include <imgui.h>
#include <cstddef>
#include <iostream>
#include <string>

void Status::reset()
{
    free_play                 = true;
    king_is_selected          = false;
    waiting_for_king_move     = false;
    defender_is_selected      = false;
    waiting_for_defender_move = false;
}

void Warnings::reset()
{
    check          = false;
    checkmate      = false;
    dangerous_move = false;
};

Position get_position(const int index)
{
    return {index % 8, index / 8};
};

int get_index(const Position position)
{
    return (position.x + position.y * 8);
}

Position get_distance(Position& position_1, Position& position_2)
{
    return {position_1.x - position_2.x, position_1.y - position_2.y};
}

void draw_scope(ImU32 scope_color)
{
    ImVec2 button_min = ImGui::GetItemRectMin();
    ImVec2 button_max = ImGui::GetItemRectMax();

    ImGui::GetWindowDrawList()->AddRect(
        button_min,
        button_max,
        scope_color,
        0.0f,
        ImDrawFlags_None,
        3.f
    );
}

void load_font(const std::string& path, const float size)
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.Fonts->AddFontFromFileTTF(path.c_str(), size))
        std::cout << "Font successfully loaded : " << path << "\n";
    else
        std::cerr << "Failed to load font: " << path << " :("
                  << "\n";
}

// Textures
GLuint load_texture_from_file(const std::string& path)
{
    int            width{};
    int            height{};
    int            nb_channels{};
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nb_channels, 0);

    if (!data)
    {
        std::cerr << "Erreur de chargement de l'image : " << path << std::endl;
        return 0;
    }

    GLuint texture{};
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

bool are_equal(const ImVec4& a, const ImVec4& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

std::pair<char, char> get_board_coordinate(Position position)
{
    return {static_cast<char>('a' + position.x), static_cast<char>('8' - position.y)};
}

std::string player_turn(Color color)
{
    switch (color)
    {
    case Color::White:
        return "White";
    case Color::Black:
        return "Black";
    default:
        return "";
    }
}