#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "shader.h"
#include "texture.h"

#include <string>
#include <unordered_map>

class ResourceManager
{
 public:
    ResourceManager() = default;
    ~ResourceManager() = default;

    Shader load_shader(std::string name,
                       const char *vertex_shader_path,
                       const char *fragment_shader_path);

    Shader get_shader(std::string name);

    Texture load_texture(std::string& name,
                         std::string& file_path,
                         int alpha);

    Texture get_texture(std::string name) const;

 private:
    std::unordered_map<std::string, Shader> _shaders;
    std::unordered_map<std::string, Texture> _textures;
};

#endif
