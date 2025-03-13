#include "GameObjectManager.hpp"
#include <vector>
#include "glm/ext/matrix_transform.hpp"
#include "utils.hpp"

void clearGameObject(GameObject& gameObject)
{
    gameObject.m_modelMatrices.clear();
    gameObject.m_pieceColors.clear();
    gameObject.m_board_instance_relation.clear();
}

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
            clearGameObject(piece);
        }
    }
    // On met à jour les gameObject en fonction du board
    for (int i = 0; i < chessboard.size(); i++)
    {
        // Si la case possède une pièce, on ajoute la position de la piece et sa couleur à sa famille de piece (GameObject)
        if (chessboard[i])
        {
            Type      pieceType = chessboard[i]->get_type();
            glm::vec3 position  = world_position(get_position(i));

            m_pieces[pieceType].m_modelMatrices.push_back(glm::translate(glm::mat4(1.0f), position));
            m_pieces[pieceType].m_pieceColors.push_back(chessboard[i]->getColor());
            // keep track of board index with model matrix index
            m_pieces[pieceType].m_board_instance_relation[i] = m_pieces[pieceType].m_modelMatrices.size() - 1;
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
    m_chessboard.m_modelMatrices.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
    m_chessboard.loadMesh("chessboard/chessboard.obj", "chessboard");
    m_chessboard.setupBuffers();
}

void GameObjectManager::movePiece(std::array<std::unique_ptr<Piece>, 64>& chessboard, unsigned int from, unsigned int to, float elapsedTime, float animationStartTime, bool isAnimating)
{
    float t = (elapsedTime - animationStartTime) / animationDuration;
    if (t >= 1.0f)
    {
        t           = 1.0f;
        isAnimating = false;
    }
    // update the piece position
    // if the piece is a knight, we need to elevate it
    if (chessboard[from] && chessboard[from]->get_type() == Type::Knight)
    {
        if (t < 0.5f)
            elevation = t;
        else
            elevation = 1.0f - t;
    }

    glm::vec3 startPos   = world_position(get_position(from), elevation);
    glm::vec3 endPos     = world_position(get_position(to), elevation);
    glm::vec3 currentPos = glm::mix(startPos, endPos, t);

    for (auto& [type, piece] : m_pieces)
    {
        for (auto& [index, instance_index] : piece.m_board_instance_relation)
        {
            if (index == from)
            {
                piece.setTransform(instance_index, currentPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
                if (!isAnimating)
                {
                    piece.m_board_instance_relation[to] = instance_index;
                    piece.m_board_instance_relation.erase(from);
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