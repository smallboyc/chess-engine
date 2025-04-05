#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include "glm/ext/matrix_transform.hpp"

namespace glmax {

Camera::Camera(bool is_track_ball)
    : _is_track_ball(is_track_ball)
{
    _front = glm::normalize(calculate_direction());
}

glm::mat4 Camera::get_view_matrix()
{
    glm::vec3 center = _position + _front;
    if (_is_track_ball)
    {
        _position = calculate_direction(_radius);
        center    = _target;
    }
    return glm::lookAt(_position, center, _up);
}

void Camera::process_mouse_movement(double xpos, double ypos)
{
    if (_first_mouse)
    {
        _last_x      = xpos;
        _last_y      = ypos;
        _first_mouse = false;
    }

    float xoffset = xpos - _last_x;
    float yoffset = ypos - _last_y;
    _last_x       = xpos;
    _last_y       = ypos;

    _yaw += xoffset * _sensitivity;
    _pitch -= yoffset * _sensitivity;

    if (_is_track_ball)
    {
        if (_pitch > 45.0f)
            _pitch = 45.0f;
        if (_pitch < 0.0f)
            _pitch = 0.0f;
    }
    else
    {
        if (_pitch > 20.0f)
            _pitch = 20.0f;
        if (_pitch < -90.0f)
            _pitch = -90.0f;
    }

    if (!_is_track_ball)
    {
        _front = glm::normalize(calculate_direction());
    }
}

void Camera::process_scroll(double yoffset)
{
    _radius -= yoffset * 0.1f;
    if (_radius < 1.0f)
        _radius = 1.0f;
    if (_radius > 32.0f)
        _radius = 32.0f;
}

void Camera::process_input(int key, int action)
{
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        _position += _sensitivity * _front;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        _position -= _sensitivity * _front;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        _position -= glm::normalize(glm::cross(_front, _up)) * _sensitivity;
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        _position += glm::normalize(glm::cross(_front, _up)) * _sensitivity;
    }
}

void Camera::set_track_ball(bool is_track_ball)
{
    _is_track_ball = is_track_ball;
};

// callbacks
void Camera::free_move_callback(int key, int action)
{
    // if (!is_track_ball() && !_is_locked)
    //     process_input(key, action);
}

void Camera::zoom_callback(double yoffset)
{
    if (!_is_locked)
        process_scroll(yoffset);
}

void Camera::track_ball_move_callback(double xpos, double ypos)
{
    if (!_is_locked)
        process_mouse_movement(xpos, ypos);
}

void Camera::active_camera_piece_view(float yaw)
{
    if (!_first_update_camera_piece_view)
    {
        _first_update_camera_track_ball = false;
        set_camera_piece_orientation(yaw);
        _first_update_camera_piece_view = true;
    }
}
void Camera::active_camera_track_ball()
{
    if (!_first_update_camera_track_ball)
    {
        _first_update_camera_piece_view = false;
        _is_track_ball                  = true;
        reset_camera_track_ball();
        _first_update_camera_track_ball = true;
    }
}

void Camera::set_camera_piece_orientation(const float& yaw)
{
    _yaw   = yaw;
    _pitch = -5.0f;
    _front = glm::normalize(calculate_direction());
};

void Camera::reset_camera_track_ball()
{
    _position = glm::vec3(0.0f, 1.0f, 0.0f);
    _target   = glm::vec3(0.0f, 0.0f, 0.0f);
    _front    = glm::vec3(1.0f, 0.0f, 1.0f);
    _up       = glm::vec3(0.0f, 1.0f, 0.0f);
    _pitch    = 30.0f;
};
} // namespace glmax
