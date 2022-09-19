#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class OrbitCamera
{
 public:
    OrbitCamera(glm::vec3 target, float radius);
    ~OrbitCamera() = default;

    void set_target(glm::vec3 target);
    void set_pitch(float pitch);
    void set_yaw(float yaw);
    void set_radius(float radius);

    glm::mat4 get_view_matrix();

 private:
    glm::vec3 _target;
    float _pitch;
    float _yaw;
    float _radius;
};

#endif