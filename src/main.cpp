#include <imgui.h>
#include <glm/glm.hpp>
#include <quick_imgui/quick_imgui.hpp>
#include "Piece.hpp"
#include "Shader.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

int main()
{
    // basic camera variables (test) next use the Camera class
    glm::vec3 _position = glm::vec3(0.0f, 2.0f, 12.0f);
    glm::vec3 _target   = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 _front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _up       = glm::vec3(0.0f, 1.0f, 0.0f);
    //
    glmax::Shader shader{};
    Piece         piece{};
    Piece         bishop{};
    quick_imgui::loop(
        "Chess",
        /* init: */
        [&]() {
            shader.loadShader("model.vs.glsl", "model.fs.glsl");
            piece.m_mesh.loadMesh("chessboard/chessboard.obj", "chessboard");
            piece.setup_buffers();
            bishop.m_mesh.loadMesh("bishop/bishop.obj", "bishop");
            bishop.setup_buffers();
        },
        /* loop: */
        [&]() {
            glClearColor(0.847f, 0.82f, 0.929f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            // Matrices de transformation
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1280) / static_cast<float>(720), 0.1f, 100.0f);
            shader.setUniformMatrix4fv("view", glm::lookAt(_position, _position + _front, _up)); // Camera.get_view_matrix()
            shader.setUniformMatrix4fv("projection", projection);

            // Définir les propriétés de la lumière et de la caméra
            shader.setUniform3fv("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
            shader.setUniform3fv("viewPos", _position); // Camera.get_position()
            shader.setUniform3fv("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

            // Render pieces / board
            shader.use();
            piece.setTransform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
            piece.render(shader);
            bishop.setTransform(glm::vec3(-3.5f, 0.0f, -3.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
            bishop.render(shader);
            ImGui::Begin("Test");
            ImGui::Text("Hello Test");
            ImGui::End();
        }
    );
}
