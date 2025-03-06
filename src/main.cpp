#include <imgui.h>
#include <glm/glm.hpp>
#include <quick_imgui/quick_imgui.hpp>
#include "Camera.hpp"
#include "PieceManager.hpp"
#include "Shader.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
#include "utils.hpp"

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
    glmax::Shader shader{};
    PieceManager  pawn_manager(Type::PAWN);
    PieceManager  chessboard(Type::CHESSBOARD);
    auto          window = quick_imgui::WindowWrapper("Quick ImGui", [&]() {
        shader.loadShader("model.vs.glsl", "model.fs.glsl");
        chessboard.m_mesh.loadMesh("chessboard/chessboard.obj", "chessboard");
        chessboard.setup_buffers();
        pawn_manager.m_mesh.loadMesh("pawn/pawn.obj", "pawn");
        pawn_manager.setup_buffers();
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

        // Render pieces / board
        shader.use();
        chessboard.render(shader);
        pawn_manager.setTransform(0, world_position({0, 1}), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
        pawn_manager.render(shader);
        ImGui::Begin("Test");
        ImGui::Text("Hello Test");
        ImGui::End();
    });
}