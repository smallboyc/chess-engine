#pragma once
#include <glm/glm.hpp>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

namespace glmax {

class Camera {
private:
    glm::vec3 _position = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 _target   = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 _front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _up       = glm::vec3(0.0f, 1.0f, 0.0f);

    float _yaw    = 0.0f;
    float _pitch  = 0.0f;
    float _radius = 5.0f;

    float _sensitivity = 0.2f;
    float _last_x      = 0.0f;
    float _last_y      = 0.0f;
    
    bool  _first_mouse = true;
    bool _is_track_ball;

public:
    Camera(bool is_track_ball)
        : _is_track_ball(is_track_ball){};

    glm::mat4 get_view_matrix();
    glm::vec3 get_position() { return _position; };
    void      process_mouse_movement(double xpos, double ypos);
    void      process_scroll(double yoffset);
    void      process_input(int key, int action);
    //
    bool is_track_ball() { return _is_track_ball; };
};

} // namespace max
