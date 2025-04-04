#pragma once
#include "Animation.hpp"
#include "GameObject.hpp"
#include "game2D/Piece.hpp"

struct Box {
    glm::vec3 min{};
    glm::vec3 max{};
};

struct Chessboard3D {
    GameObject                   model;
    std::unordered_map<int, Box> boxes;
    void save_box(int index);
};

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
    // GameObject                           m_chessboard;
    Chessboard3D m_chessboard;
};
