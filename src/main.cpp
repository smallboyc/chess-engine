#include <imgui.h>
#include <glm/glm.hpp>
#include <quick_imgui/quick_imgui.hpp>
#include "Piece.hpp"
#include "Shader.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"

int main()
{
    // basic camera variables (test) next use the Camera class
    glm::vec3 _position = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 _target   = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 _front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _up       = glm::vec3(0.0f, 1.0f, 0.0f);
    //
    glmax::Shader shader{};
    Piece         piece{};
    quick_imgui::loop(
        "Chess",
        /* init: */
        [&]() {
            shader.loadShader("/Users/smallboyc/Documents/IMAC/imac2/S4/OpenGL/chess-engine/shaders/model.vs.glsl", "/Users/smallboyc/Documents/IMAC/imac2/S4/OpenGL/chess-engine/shaders/model.fs.glsl");
            piece._mesh.loadMesh("/Users/smallboyc/Documents/IMAC/imac2/S4/OpenGL/chess-engine/assets/models/special_block/special_block.obj", "/Users/smallboyc/Documents/IMAC/imac2/S4/OpenGL/chess-engine/assets/models/special_block");
            piece.setup_buffers();
        },
        /* loop: */
        [&]() {
            glClearColor(0.847f, 0.82f, 0.929f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            // Matrices de transformation
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1280) / static_cast<float>(720), 0.1f, 100.0f);

            // Envoi des matrices au shader
            // test
            glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(glm::lookAt(_position, _position + _front, _up)));
            // glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(Camera.get_view_matrix()));
            glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            // Lumière
            GLint lightPosLoc   = glGetUniformLocation(shader.getID(), "lightPos");
            GLint viewPosLoc    = glGetUniformLocation(shader.getID(), "viewPos");
            GLint lightColorLoc = glGetUniformLocation(shader.getID(), "lightColor");

            // Définir les propriétés de la lumière et de la caméra
            glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 5.0f);
            // test
            glm::vec3 viewPos = _position;
            // glm::vec3 viewPos    = Camera.get_position();
            glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

            glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
            // glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));
            glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

            shader.use();
            piece.setTransform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(glm::radians(20.0f), glm::radians(45.0f), 0.0f), glm::vec3(1.0f));
            piece.draw(shader); // Utilisation du shader
        }
    );
}
