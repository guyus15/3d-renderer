#include "resource_manager.h"

#include "stb/stb_image.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::unordered_map<std::string, Shader> ResourceManager::_shaders;
std::unordered_map<std::string, Texture2D> ResourceManager::_textures;

Shader ResourceManager::load_shader(const std::string& name,
                                    const char *vertex_shader_path,
                                    const char *fragment_shader_path)
{
    _shaders[name] = load_shader_from_file(vertex_shader_path, fragment_shader_path);
    return _shaders[name];
}

Shader ResourceManager::get_shader(const std::string& name)
{
    return _shaders[name];
}

Texture2D ResourceManager::load_texture(const std::string& name, const char *file_path, bool alpha)
{
    _textures[name] = load_texture_from_file(file_path, alpha);
    return _textures[name];
}
Texture2D ResourceManager::get_texture(const std::string& name)
{
    return _textures[name];
}

Shader ResourceManager::load_shader_from_file(const char *vertex_shader_path,
                                    const char *fragment_shader_path)
{
    std::string vertex_code, fragment_code;
    std::ifstream vertex_shader_file, fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertex_shader_file.open(vertex_shader_path);
        fragment_shader_file.open(fragment_shader_path);

        std::stringstream vertex_shader_stream, fragment_shader_stream;

        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        vertex_code = vertex_shader_stream.str();
        fragment_code = fragment_shader_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "Error:: Shader file could not be read successfully." << std::endl;
    }

    Shader shader{};
    shader.compile(vertex_code, fragment_code);

    return shader;
}
Texture2D ResourceManager::load_texture_from_file(const char* file, bool alpha)
{
    Texture2D texture;

    if (alpha)
    {
        texture.enable_alpha(true);
    }

    int width, height, no_channels;
    unsigned char *data = stbi_load(file, &width, &height, &no_channels, 0);

    texture.generate(width, height, data);

    stbi_image_free(data);

    return texture;
}
