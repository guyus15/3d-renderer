#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "glad/glad.h"

#include <string>

class Texture2D
{
 public:
    Texture2D();
    ~Texture2D() = default;

    void enable_alpha(bool enable);
    void generate(int width, int height, unsigned char *data);
    void bind() const;

 private:
    GLuint _texture_id{};
    GLint _internal_format;
    GLint _image_format;
    GLint _wrap_s, _wrap_t;
    GLint _filter_min, _filter_mag;
    GLsizei _width{}, _height{};
};

#endif
