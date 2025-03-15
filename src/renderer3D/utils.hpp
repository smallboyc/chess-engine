#pragma once
#include <glm/glm.hpp>


namespace Renderer3D
{
    inline glm::vec2 get_position(const unsigned int index)
    {
        return {index % 8, index / 8};
    };

    inline glm::vec3 world_position(glm::vec2 position, float elevation = 0.0f)
    {
        return {-3.5f + position.x, 2 * elevation, -3.5f + position.y};
    }
} // namespace Renderer3D
