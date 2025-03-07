#pragma once
#include "GameObject.hpp"
#include "Piece.hpp"


class PieceManager {
public:
    PieceManager()                                     = default;
    PieceManager(const PieceManager& other)            = delete;
    PieceManager& operator=(const PieceManager& other) = delete;
    ~PieceManager()                                    = default;
    void                                   init(std::array<std::unique_ptr<Piece>, 64>& chessboard);
    void                                   loadAllMesh();
    std::unordered_map<Type, GameObject>   m_gameObjects;
    GameObject                             m_chessboard;
    //
    // Previous chessboard
    std::array<std::unique_ptr<Piece>, 64> m_previous_chessboard;
};