#pragma once
#include <glm/glm.hpp>

namespace glmax {

struct Vertex {
    glm::vec3 _position;
    glm::vec3 _normal;
    glm::vec2 _tex_coord;
};

} // namespace glmax