#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include "glm/ext/matrix_transform.hpp"

namespace glmax {

Camera::Camera(bool is_track_ball)
    : _is_track_ball(is_track_ball)
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    direction.y = sin(glm::radians(_pitch));
    direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front      = glm::normalize(direction);
}

glm::mat4 Camera::get_view_matrix()
{
    if (_is_track_ball)
    {
        _position = glm::vec3(
            _radius * cos(glm::radians(_pitch)) * sin(glm::radians(_yaw)),
            _radius * sin(glm::radians(_pitch)),
            _radius * cos(glm::radians(_pitch)) * cos(glm::radians(_yaw))

        );
        return glm::lookAt(_position, _target, _up);
    }
    return glm::lookAt(_position, _position + _front, _up);
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

    if (is_track_ball())
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
        glm::vec3 direction;
        direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        direction.y = sin(glm::radians(_pitch));
        direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _front      = glm::normalize(direction);
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

// callbacks
void Camera::free_move_callback(int key, int action)
{
    // if (!is_track_ball() && !_is_locked)
    //     process_input(key, action);
}

void Camera::zoom_callback(double yoffset)
{
    // if (is_track_ball() && !_isLocked)
    if (!_is_locked)
        process_scroll(yoffset);
}

void Camera::track_ball_move_callback(double xpos, double ypos)
{
    // if (is_track_ball() && !_isLocked)
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
        is_track_ball()                 = true;
        reset_camera_track_ball();
        _first_update_camera_track_ball = true;
    }
}
} // namespace glmax
