#include <imgui.h>
#include <chrono>
#include <glm/glm.hpp>
#include <iostream>
#include <quick_imgui/quick_imgui.hpp>
#include "Camera.hpp"
#include "Chessboard.hpp"
#include "PieceManager.hpp"
#include "Shader.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

glmax::Camera Camera(true);

int window_width  = 1000;
int window_height = 700;

// Implémentations des callbacks
void key_callback(int key, int scancode, int action, int mods)
{
    if (!Camera.is_track_ball())
        Camera.process_input(key, action);
}

void mouse_button_callback(int button, int action, int mods) {}

void scroll_callback(double xoffset, double yoffset)
{
    if (Camera.is_track_ball())
        Camera.process_scroll(yoffset);
}

void cursor_position_callback(double xpos, double ypos)
{
    if (Camera.is_track_ball())
        Camera.process_mouse_movement(xpos, ypos);
}

void size_callback(int width, int height)
{
    window_width  = width;
    window_height = height;
}

int main()
{
    Chessboard    _chessboard;
    unsigned int  from = 49;
    unsigned int  to   = 33;
    glmax::Shader shader{};
    PieceManager  piece_manager;
    // Ajoutez ces variables pour suivre le temps et l'état de l'animation
    bool  isAnimating        = false;
    float animationStartTime = 0.0f;
    float animationDuration  = 2.0f; // Durée de l'animation en secondes
    bool  isFirstTime        = true;
    // Chronomètre pour mesurer le temps
    auto start_time = std::chrono::steady_clock::now(); // Démarrer le chronomètre

    auto window = quick_imgui::WindowWrapper("Quick ImGui", [&]() {
        shader.loadShader("model.vs.glsl", "model.fs.glsl");
        // All pieces
        piece_manager.init(_chessboard.m_board);
        piece_manager.loadAllMesh();
        // Chessboard
        piece_manager.m_chessboard.m_modelMatrices.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
        piece_manager.m_chessboard.loadMesh("chessboard/chessboard.obj", "chessboard");
        piece_manager.m_chessboard.setupBuffers();
    });

    // callbacks
    window.key_callback             = key_callback;
    window.size_callback            = size_callback;
    window.cursor_position_callback = cursor_position_callback;
    window.scroll_callback          = scroll_callback;
    window.mouse_button_callback    = mouse_button_callback;

    window.run([&]() {
        glClearColor(0.847f, 0.82f, 0.929f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1280) / static_cast<float>(720), 0.1f, 100.0f);
        shader.setUniformMatrix4fv("view", Camera.get_view_matrix());
        shader.setUniformMatrix4fv("projection", projection);

        // Définir les propriétés de la lumière et de la caméra
        shader.setUniform3fv("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
        shader.setUniform3fv("viewPos", Camera.get_position());
        shader.setUniform3fv("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        // Calculer le temps écoulé depuis le début du jeu
        auto                         current_time = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed      = current_time - start_time;
        float                        elapsed_time = elapsed.count();

        // Démarrez l'animation après 12 secondes
        if (elapsed_time > 12.0f && !isAnimating && isFirstTime)
        {
            isAnimating        = true;
            animationStartTime = elapsed_time;
            isFirstTime        = false;
        }

        // UPDATE PIECE POSITION
        if (isAnimating)
        {
            float t = (elapsed_time - animationStartTime) / animationDuration;
            if (t >= 1.0f)
            {
                t           = 1.0f;
                isAnimating = false; // Animation terminée
            }

            glm::vec3 startPos   = world_position(get_position(from));
            glm::vec3 endPos     = world_position(get_position(to));
            glm::vec3 currentPos = glm::mix(startPos, endPos, t);

            for (auto& [type, gameObject] : piece_manager.m_gameObjects)
            {
                for (auto& [index, instance_index] : gameObject.m_board_instance_relation)
                {
                    if (index == from)
                    {
                        gameObject.setTransform(instance_index, currentPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
                        if (!isAnimating)
                        {
                            gameObject.m_board_instance_relation[to] = instance_index;
                            gameObject.m_board_instance_relation.erase(from);
                        }
                    }
                }
            }
        }

        // RENDER CHESSBOARD AND PIECES
        shader.use();
        piece_manager.m_chessboard.render(shader);

        for (auto& [type, gameObject] : piece_manager.m_gameObjects)
        {
            gameObject.render(shader);
        }

        ImGui::Begin("Test");
        ImGui::Text("Hello Test");
        ImGui::End();
    });
}
