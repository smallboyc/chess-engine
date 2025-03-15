#include "GameObjectManager.hpp"
#include <iostream>
#include "game2D/Piece.hpp"
#include "glm/fwd.hpp"
#include "utils.hpp"

void GameObjectManager::updatePiecesData()
{
    for (auto& [type, piece] : m_pieces)
    {
        piece.updateMatInstancingBuffer();
        piece.updateColorInstancingBuffer();
    }
}

// A chaque frame, on met à jour l'affichage de notre jeu 3D, en analysant le board envoyé par le jeu 2D.
void GameObjectManager::updatePiecesPositions(std::array<std::unique_ptr<Piece>, 64>& chessboard)
{
    // On vide toutes les positions des pieces
    if (!m_pieces.empty())
    {
        for (auto& [type, piece] : m_pieces)
        {
            piece.clearInstancingBuffers();
        }
    }
    // On met à jour les gameObject en fonction du board
    for (int i = 0; i < chessboard.size(); i++)
    {
        // Si la case possède une pièce, on ajoute la position de la piece et sa couleur à sa famille de piece (GameObject)
        if (chessboard[i])
        {
            Type      pieceType = chessboard[i]->get_type();
            glm::vec3 position  = Renderer3D::world_position(Renderer3D::get_position(i));
            glm::vec3 color     = chessboard[i]->get_vec_color();
            //
            m_pieces[pieceType].pushMatrix(position);
            m_pieces[pieceType].pushColor(color);
            // WIP ? keep track of board index with model matrix index
            m_pieces[pieceType].updateBoardRelations(i);
        }
        // Si la case est vide, on vérifie si pour chaque gameObject, i est dans m_board_instance_relation, dans ce cas on retire de m_board_instance_relation et de m_modelMatrices.
    }
}

void GameObjectManager::loadAllPieces()
{
    for (auto& [type, piece] : m_pieces)
    {
        if (type == Type::Pawn)
            piece.loadMesh("pawn/pawn.obj", "pawn");
        else if (type == Type::Bishop)
            piece.loadMesh("bishop/bishop.obj", "bishop");
        else if (type == Type::Rook)
            piece.loadMesh("rook/rook.obj", "rook");
        else if (type == Type::Knight)
            piece.loadMesh("knight/knight.obj", "knight");
        else if (type == Type::Queen)
            piece.loadMesh("queen/queen.obj", "queen");
        else if (type == Type::King)
            piece.loadMesh("king/king.obj", "king");
        piece.setupBuffers();
    }
}

void GameObjectManager::loadChessboard()
{
    glm::vec3 centerPos{glm::vec3(0.0f, 0.0f, 0.0f)};
    m_chessboard.pushMatrix(centerPos);
    m_chessboard.loadMesh("chessboard/chessboard.obj", "chessboard");
    m_chessboard.setupBuffers();
}

// This method is about to change (from & to logic not ok)
void GameObjectManager::movePiece(std::array<std::unique_ptr<Piece>, 64>& chessboard, MoveProcessing& moveProcessing, float& elapsedTime, float& animationStartTime, bool& isAnimating)
{
    auto [from, to] = moveProcessing;
    float t         = (elapsedTime - animationStartTime) / animationDuration;
    // std::cout << animationStartTime << "\n";
    if (t >= 1.0f)
    {
        t = 1.0f;
        // end of animation
        isAnimating = false;
    }
    // update the piece position
    // if the piece is a knight, we need to elevate it
    if (isAnimating)
    {
        if (chessboard[to] && chessboard[to]->get_type() == Type::Knight)
        {
            if (t < 0.5f)
                elevation = t;
            else
                elevation = 1.0f - t;
        }

        glm::vec3 startPos   = Renderer3D::world_position(Renderer3D::get_position(from), elevation);
        glm::vec3 endPos     = Renderer3D::world_position(Renderer3D::get_position(to), elevation);
        glm::vec3 currentPos = glm::mix(startPos, endPos, t);

        for (auto& [type, piece] : m_pieces)
        {
            for (auto& [index, instance_index] : piece.m_board_instance_relation)
            {
                if (index == from)
                {
                    piece.setTransform(instance_index, currentPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
                }
            }
        }
    }
}

void GameObjectManager::renderGameObjects(glmax::Shader& shader)
{
    m_chessboard.render(shader);

    for (auto& [type, piece] : m_pieces)
    {
        piece.render(shader);
    }
}