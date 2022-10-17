#ifndef MESH_H
#define MESH_H

#include "shader.h"

#include "glm/glm.hpp"

#include <string>
#include <vector>

struct Vertex
{
	glm::vec3 _position;
	glm::vec3 _normal;
	glm::vec2 _tex_coords;
};

struct Texture
{
	unsigned int _id;
	std::string _type;
	std::string _path;
};

class Mesh
{
public:
	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;
	std::vector<Texture> _textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> texture);
	void draw(Shader& shader);

private:
	unsigned int _VAO, _VBO, _EBO;

	void setup_mesh();
};

#endif