#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
 public:
    explicit Texture(const std::string& texture_path);
    ~Texture() = default;

    void bind();

 private:
    unsigned int _texture_id;
    unsigned int _width, _height;
    unsigned int _internal_format;
    unsigned int _image_format;
    unsigned int _wrap_s, _wrap_t;
    unsigned int _filter_min, _filter_max;
};

#endif
