#include "texture.h"

#include "glad/glad.h"

#include <fstream>
#include <iostream>
#include <sstream>

Texture::Texture(const std::string& texture_path)
{
    _internal_format = GL_RGB;
    _image_format = GL_RGB;
    _wrap_s = GL_REPEAT;
    _wrap_t = GL_REPEAT;
    _filter_min = GL_LINEAR;
    _filter_max = GL_LINEAR;
}
