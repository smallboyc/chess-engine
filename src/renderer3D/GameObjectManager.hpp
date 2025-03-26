#pragma once
#include "Animation.hpp"
#include "GameObject.hpp"
#include "game2D/Piece.hpp"

// A class that has control over all gameobjects
class GameObjectManager {
public:
    GameObjectManager()                                          = default;
    GameObjectManager(const GameObjectManager& other)            = delete;
    GameObjectManager& operator=(const GameObjectManager& other) = delete;
    ~GameObjectManager()                                         = default;
    void update_pieces_positions(std::array<std::unique_ptr<Piece>, 64>& chessboard);
    void update_pieces_data();
    void load_all_pieces();
    void load_chessboard();
    void move_piece(std::array<std::unique_ptr<Piece>, 64>& chessboard, MoveProcessing& moveProcessing, float& elapsedTime, Animation& animation);
    void render_game_objects(glmax::Shader& shader, Settings& settings);

private:
    std::unordered_map<Type, GameObject> m_pieces;
    GameObject                           m_chessboard;
    float                                animation_duration{2.0f};
    float                                elevation{0.0f};
};
