#include "orbit_camera.h"

#include "glm/ext/matrix_transform.hpp"

OrbitCamera::OrbitCamera(glm::vec3 target, float radius)
    : _target{target}, _pitch{0.0f}, _yaw{0.0f}, _radius{radius}
{}

void OrbitCamera::set_target(glm::vec3 target)
{
    _target = target;
}

void OrbitCamera::set_pitch(float pitch)
{
    _pitch = pitch;

    // Clamp pitch to 90 degrees positive and negative.
    if (_pitch > 89.9f)
    {
        _pitch = 89.9f;
    }
    else if (_pitch < -89.9f)
    {
        _pitch = -89.9f;
    }
}

void OrbitCamera::set_yaw(float yaw)
{
    _yaw = yaw;
}

void OrbitCamera::set_radius(float radius)
{
    _radius = radius;
}

glm::mat4 OrbitCamera::get_view_matrix()
{
    // For now, we will ignore pitch movement.

    glm::vec3 world_up = glm::vec3{0.0f, 1.0f, 0.0f};

    glm::mat4 view = glm::lookAt
        (
        glm::vec3{sin(glm::radians(_yaw)) * _radius, 0.0f, cos(glm::radians(_yaw)) * _radius},
        _target,
        world_up
        );

    return view;
}

