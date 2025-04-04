#pragma once
#include <glm/glm.hpp>
#include "glm/fwd.hpp"

namespace glmax {

class Camera {
private:
    glm::vec3 _position = glm::vec3(0.0f, 1.0f, 0.0f);
    // glm::vec3 _position = glm::vec3(-3.5f, 1.0f, -3.5f); // camera position
    glm::vec3 _target = glm::vec3(0.0f, 0.0f, 0.0f); // where is looking the camera at (origin)
    glm::vec3 _front  = glm::vec3(1.0f, 0.0f, 1.0f); // direction of the camera (vector direction)
    glm::vec3 _up     = glm::vec3(0.0f, 1.0f, 0.0f); // up direction of the camera (vector direction) by default in OpenGL (Y axis)

    float _yaw    = 90.0f; // angle around Y axis
    float _pitch  = 10.0f; // angle around X axis
    float _radius = 16.0f; // distance from the camera to the target

    float _sensitivity = 0.2f;
    float _last_x      = 0.0f;
    float _last_y      = 0.0f;

    bool _first_mouse = true;
    bool _is_track_ball;
    bool _is_locked = false;

    // first update : used to change once the properties of the camera
    bool _first_update_camera_piece_view = false;
    bool _first_update_camera_track_ball = false;

public:
    Camera(bool is_track_ball);

    glm::mat4 get_view_matrix();
    glm::vec3 get_position() { return _position; };
    void      process_mouse_movement(double xpos, double ypos);
    void      process_scroll(double yoffset);
    void      process_input(int key, int action);
    void      set_position(const glm::vec3& position) { _position = position; }
    void      set_target(const glm::vec3& target) { _target = target; }
    void      set_front(const glm::vec3& front) { _front = front; }
    void      set_up(const glm::vec3& up) { _up = up; }
    void      set_sensitivity(float sensitivity) { _sensitivity = sensitivity; }
    //
    void toggle_lock() { _is_locked = !_is_locked; };
    bool is_locked() const { return _is_locked; };
    //
    bool& is_track_ball() { return _is_track_ball; };
    //
    // callbacks
    void free_move_callback(int key, int action);
    void zoom_callback(double yoffset);
    void track_ball_move_callback(double xpos, double ypos);
    //
    void active_camera_piece_view(float yaw);
    void active_camera_track_ball();
    void set_camera_piece_position(const glm::vec3& position)
    {
        _position = {position.x, 1.4f, position.z};
    };
    void set_camera_piece_orientation(const float& yaw)
    {
        _yaw   = yaw;
        _pitch = -5.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        direction.y = sin(glm::radians(_pitch));
        direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _front      = glm::normalize(direction);
    };
    void reset_camera_track_ball()
    {
        _position = glm::vec3(0.0f, 1.0f, 0.0f);
        _target   = glm::vec3(0.0f, 0.0f, 0.0f);
        _front    = glm::vec3(1.0f, 0.0f, 1.0f);
        _up       = glm::vec3(0.0f, 1.0f, 0.0f);
        _pitch    = 30.0f;
    };
};

} // namespace glmax
