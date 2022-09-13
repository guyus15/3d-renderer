#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

enum class CameraMovement
{
    FORWARD = 0,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
 public:
    Camera(glm::vec3 position = glm::vec3{0.0f, 0.0f, 0.0f},
           glm::vec3 up = glm::vec3{0.0f, 1.0f, 0.0f},
           float yaw = YAW,
           float pitch = PITCH);
    Camera(float posx, float posy, float posz, float upx, float upy, float upz, float yaw, float pitch);

    glm::mat4 get_view_matrix();

    void process_keyboard(CameraMovement direction, float delta_time);
    void process_mouse_movement(float xoffset, float yoffset, GLboolean constrain_pitch = true);
    void process_mouse_scroll(float yoffset);

 private:
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _world_up;

    float _yaw;
    float _pitch;
    float _movement_speed;
    float _mouse_sensitivity;
    float _zoom;

    void update_camera_vectors();
};

#endif