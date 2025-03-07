#include "PieceManager.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "utils.hpp"

void PieceManager::init(std::array<std::unique_ptr<Piece>, 64>& chessboard)
{
    for (int i = 0; i < chessboard.size(); i++)
    {
        if (chessboard[i])
        {
            Type      pieceType = chessboard[i]->get_type();
            glm::vec3 position  = world_position(get_position(i));

            m_gameObjects[pieceType].m_modelMatrices.push_back(glm::translate(glm::mat4(1.0f), position));
            m_gameObjects[pieceType].m_pieceColors.push_back(chessboard[i]->getColor());
            // keep track of board index with model matrix index
            m_gameObjects[pieceType].m_board_instance_relation[i] = m_gameObjects[pieceType].m_modelMatrices.size() - 1;
        }
    }
}

void PieceManager::loadAllMesh()
{
    for (auto& [type, gameObject] : m_gameObjects)
    {
        if (type == Type::Pawn)
            gameObject.loadMesh("pawn/pawn.obj", "pawn");
        else if (type == Type::Bishop)
            gameObject.loadMesh("bishop/bishop.obj", "bishop");
        else if (type == Type::Rook)
            gameObject.loadMesh("rook/rook.obj", "rook");
        else if (type == Type::Knight)
            gameObject.loadMesh("knight/knight.obj", "knight");
        else if (type == Type::Queen)
            gameObject.loadMesh("queen/queen.obj", "queen");
        if (type == Type::King)
            gameObject.loadMesh("king/king.obj", "king");
        gameObject.setupBuffers();
    }
}
