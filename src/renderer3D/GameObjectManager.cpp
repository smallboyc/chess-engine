#include "GameObjectManager.hpp"
#include <iostream>
#include "game2D/Piece.hpp"
#include "glm/fwd.hpp"
#include "utils.hpp"

void GameObjectManager::update_pieces_data()
{
    for (auto& [type, piece] : m_pieces)
    {
        piece.update_mat_instancing_buffer();
        piece.update_color_instancing_buffer();
    }
}

// A chaque frame, on met à jour l'affichage de notre jeu 3D, en analysant le board envoyé par le jeu 2D.
void GameObjectManager::update_pieces_positions(std::array<std::unique_ptr<Piece>, 64>& chessboard)
{
    // On vide toutes les positions des pieces
    if (!m_pieces.empty())
    {
        for (auto& [type, piece] : m_pieces)
        {
            piece.clear_instancing_buffers();
        }
    }
    // On met à jour les gameObject en fonction du board
    for (int i = 0; i < chessboard.size(); i++)
    {
        // Si la case possède une pièce, on ajoute la position de la piece et sa couleur à sa famille de piece (GameObject)
        if (chessboard[i])
        {
            Type      pieceType = chessboard[i]->get_type();
            glm::vec3 position  = Renderer3DUtils::world_position(Renderer3DUtils::get_position(i));
            glm::vec3 color     = chessboard[i]->get_vec_color();
            //
            m_pieces[pieceType].push_matrix(position);
            m_pieces[pieceType].push_color(color);
            //
            m_pieces[pieceType].update_board_relations(i);
        }
    }
}

void GameObjectManager::load_all_pieces()
{
    for (auto& [type, piece] : m_pieces)
    {
        if (type == Type::Pawn)
            piece.load_mesh("pawn/pawn.obj", "pawn");
        else if (type == Type::Bishop)
            piece.load_mesh("bishop/bishop.obj", "bishop");
        else if (type == Type::Rook)
            piece.load_mesh("rook/rook.obj", "rook");
        else if (type == Type::Knight)
            piece.load_mesh("knight/knight.obj", "knight");
        else if (type == Type::Queen)
            piece.load_mesh("queen/queen.obj", "queen");
        else if (type == Type::King)
            piece.load_mesh("king/king.obj", "king");
        piece.setup_buffers();
    }
}

void GameObjectManager::load_chessboard()
{
    glm::vec3 center_position{glm::vec3(0.0f, 0.0f, 0.0f)};
    m_chessboard.push_matrix(center_position);
    m_chessboard.load_mesh("chessboard/chessboard.obj", "chessboard");
    m_chessboard.setup_buffers();
}

void GameObjectManager::move_piece(std::array<std::unique_ptr<Piece>, 64>& chessboard, MoveProcessing& moveProcessing, float& elapsedTime, Animation& animation)
{
    auto [from, to] = moveProcessing;
    float t         = (elapsedTime - animation.animation_start_time) / animation.animation_duration;
    // std::cout << animationStartTime << "\n";
    if (t >= 1.0f)
    {
        t = 1.0f;
        // end of animation
        animation.is_animating = false;
    }
    // update the piece position
    // if the piece is a knight, we need to elevate it
    if (animation.is_animating)
    {
        if (chessboard[to] && chessboard[to]->get_type() == Type::Knight)
        {
            if (t < 0.5f)
                animation.elevation = t;
            else
                animation.elevation = 1.0f - t;
        }

        glm::vec3 start_position   = Renderer3DUtils::world_position(Renderer3DUtils::get_position(from), animation.elevation);
        glm::vec3 end_position     = Renderer3DUtils::world_position(Renderer3DUtils::get_position(to), animation.elevation);
        glm::vec3 current_position = glm::mix(start_position, end_position, t);

        for (auto& [type, piece] : m_pieces)
        {
            for (const auto& [index, instance_index] : piece.get_board_instance_relation())
            {
                if (index == from)
                {
                    piece.set_transform(instance_index, current_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
                }
            }
        }
    }
}

void GameObjectManager::render_game_objects(glmax::Shader& shader, Settings& settings)
{
    m_chessboard.render(shader, settings);

    for (auto& [type, piece] : m_pieces)
    {
        piece.render(shader, settings);
    }
}