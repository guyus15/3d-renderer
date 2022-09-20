#include "texture.h"

#include "glad/glad.h"

#include <iostream>

Texture::Texture()
{
    _internal_format = GL_RGB;
    _image_format = GL_RGB;
    _wrap_s = GL_REPEAT;
    _wrap_t = GL_REPEAT;
    _filter_min = GL_LINEAR;
    _filter_mag = GL_LINEAR;
}
void Texture::enable_alpha(bool enable)
{
    if (enable)
    {
        _internal_format = GL_RGBA;
        _image_format = GL_RGBA;
    } else
    {
        _internal_format = GL_RGB;
        _image_format = GL_RGB;
    }
}
void Texture::generate(int width, int height, unsigned char *data)
{
    glGenTextures(1, &_texture_id);
    glBindTexture(GL_TEXTURE_2D, _texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter_mag);

    _width = width;
    _height = height;

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, _internal_format, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cerr << "Error: Failed to generate texture." << std::endl;
        return;
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, _texture_id);
}
