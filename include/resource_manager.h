#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "shader.h"
#include "texture2d.h"
#include "model.h"

#include <string>
#include <unordered_map>

class ResourceManager
{
 public:
    static Shader load_shader(const std::string& name,
                       const char *vertex_shader_path,
                       const char *fragment_shader_path);

    static Shader get_shader(const std::string& name);

    static Texture2D load_texture(const std::string& name,
                         const char *file_path,
                         bool alpha,
                         bool flip_on_load = false);

    static Texture2D get_texture(const std::string& name);

    static Model load_model(const std::string& name,
                            const std::string& file_path);

    static Model get_model(const std::string& name);

 private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    static Shader load_shader_from_file(const char *vertex_shader_path,
                                 const char *fragment_shader_path);

    static Texture2D load_texture_from_file(const char *file, bool alpha, bool flip_on_load);

    static Model load_model_from_file(const std::string& file);

    static std::unordered_map<std::string, Shader> _shaders;
    static std::unordered_map<std::string, Texture2D> _textures;
    static std::unordered_map<std::string, Model> _models;
};

#endif
