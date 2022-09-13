#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
: _front{glm::vec3{0.0f, 0.0f, -1.0f}},
    _movement_speed{SPEED},
    _mouse_sensitivity{SENSITIVITY},
    _zoom{ZOOM}
{
    _position = position;
    _world_up = up;
    _yaw = yaw;
    _pitch = pitch;
    update_camera_vectors();
}

Camera::Camera(float posx, float posy, float posz, float upx, float upy, float upz, float yaw, float pitch)
: _front{glm::vec3{0.0f, 0.0f, -1.0f}},
    _movement_speed{SPEED},
    _mouse_sensitivity{SENSITIVITY},
    _zoom{ZOOM}
{
    _position = glm::vec3{posx, posy, posz};
    _world_up = glm::vec3{upx, upy, upz};
    _yaw = yaw;
    _pitch = pitch;
    update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::process_keyboard(CameraMovement direction, float delta_time)
{
    float velocity = _movement_speed * delta_time;

    if (direction == CameraMovement::FORWARD)
    {
        _position += _front * velocity;
    }
    if (direction == CameraMovement::BACKWARD)
    {
        _position -= _front * velocity;
    }
    if (direction == CameraMovement::LEFT)
    {
        _position -= _right * velocity;
    }
    if (direction == CameraMovement::RIGHT)
    {
        _position += _right * velocity;
    }
}

void Camera::process_mouse_movement(float xoffset, float yoffset, GLboolean constrain_pitch)
{
    xoffset *= _mouse_sensitivity;
    yoffset *= _mouse_sensitivity;

    _yaw += xoffset;
    _pitch += yoffset;

    if (constrain_pitch)
    {
        if (_pitch > 89.0f)
        {
            _pitch = 89.0f;
        }
        if (_pitch < -89.0f)
        {
            _pitch = -89.0f;
        }
    }

    update_camera_vectors();
}

void Camera::process_mouse_scroll(float yoffset)
{
    _zoom -= (float)yoffset;

    if (_zoom < 1.0f)
    {
        _zoom = 1.0f;
    }
    if (_zoom > 45.0f)
    {
        _zoom = 45.0f;
    }
}

void Camera::update_camera_vectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _world_up));
    _up = glm::normalize(glm::cross(_right, _front));
}