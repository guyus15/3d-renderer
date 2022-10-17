#include "mesh.h"

#include "glad/glad.h"

#include <string>
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: _vertices{ vertices }, _indices{ indices }, _textures{ textures }
{
	setup_mesh();
}

void Mesh::draw(Shader& shader)
{
	unsigned int diffuse_nr = 1;
	unsigned int specular_nr = 1;
	unsigned int normal_nr = 1;
	unsigned int height_nr = 1;

	for (unsigned int i = 0; i < _textures.size(); i++)
	{
		// Activate correct texture unit before binding.
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = _textures[i]._type;

		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuse_nr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specular_nr++);
		}
		else if (name == "texture_normal")
		{
			number = std::to_string(normal_nr++);
		}
		else if (name == "texture_height")
		{
			number = std::to_string(height_nr++);
		}

		shader.set_int((name + number), i);
		glBindTexture(GL_TEXTURE_2D, _textures[i]._id);
	}

	glActiveTexture(GL_TEXTURE0);

	// Draw mesh
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(_indices.size()), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setup_mesh()
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _normal));

	// Vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _tex_coords));

	glBindVertexArray(0);
}