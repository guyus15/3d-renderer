#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "shader.h"
#include "texture.h"

#include <string>
#include <unordered_map>

class ResourceManager
{
 public:
    static Shader load_shader(const std::string& name,
                       const char *vertex_shader_path,
                       const char *fragment_shader_path);

    static Shader get_shader(const std::string& name);

    static Texture load_texture(const std::string& name,
                         const char *file_path,
                         bool alpha);

    static Texture get_texture(const std::string& name);

 private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    static Shader load_shader_from_file(const char *vertex_shader_path,
                                 const char *fragment_shader_path);

    static Texture load_texture_from_file(const char *file, bool alpha);

    static std::unordered_map<std::string, Shader> _shaders;
    static std::unordered_map<std::string, Texture> _textures;
};

#endif
