#ifndef SHADER_H
#define SHADER_H

#include "glm/mat4x4.hpp"

#include <string>

class Shader
{
 public:
    Shader() = default;
    ~Shader() = default;

    void compile(const std::string& vertex_shader_code, const std::string& fragment_shader_code);
    void use();

    void set_bool(const std::string &name, bool value) const;
    void set_int(const std::string &name, int value) const;
    void set_float(const std::string &name, float value) const;
    void set_mat4(const std::string &name, glm::mat4& mat4) const;

 private:
    unsigned int _program_id;
};

#endif
