#pragma once
#include "GameObject.hpp"
#include "game2D/Piece.hpp"

class GameObjectManager {
public:
    GameObjectManager()                                          = default;
    GameObjectManager(const GameObjectManager& other)            = delete;
    GameObjectManager& operator=(const GameObjectManager& other) = delete;
    ~GameObjectManager()                                         = default;
    void initPiecesPositions(std::array<std::unique_ptr<Piece>, 64>& chessboard);
    void loadAllPieces();
    void loadChessboard();
    void movePiece(std::array<std::unique_ptr<Piece>, 64>& chessboard, unsigned int from, unsigned int to, float elapsedTime, float animationStartTime, bool isAnimating);
    void renderGameObjects(glmax::Shader& shader);

private:
    float                                animationDuration{2.0f};
    float                                elevation{0.0f};
    std::unordered_map<Type, GameObject> m_pieces;
    GameObject                           m_chessboard;
    //
    // Previous chessboard
    std::array<std::unique_ptr<Piece>, 64> m_previous_chessboard;
};