#include "model.h"

#include "glad/glad.h"
#include "stb/stb_image.h"

#include <cstdlib>
#include <iostream>
#include <memory>

unsigned int texture_from_file(const std::string& path, const std::string& directory);

Model::Model(const std::string& path)
{
	stbi_set_flip_vertically_on_load(true);
	load_model(path);
}

void Model::draw(Shader& shader)
{
	for (unsigned int i = 0; i < _meshes.size(); i++)
	{
		_meshes[i].draw(shader);
	}
}

void Model::load_model(const std::string& path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		path,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Error: Failed to import model.\n" << importer.GetErrorString() << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "Success: Loaded model " << path << " successfully." << std::endl;
	}

	_directory = path.substr(0, path.find_last_of('/'));

	process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
	// Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(process_mesh(mesh, scene));
	}

	// Then do the same for each of its children.
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		process_node(node->mChildren[i], scene);
	}
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// Process vertex positions, normals and texture coordinates.
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex._position = vector;

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex._normal = vector;
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex._tex_coords = vec;
		}
		else
		{
			vertex._tex_coords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// Process indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Process materials

	if (scene->HasMaterials())
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

		std::vector<Texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

		std::vector<Texture> normal_maps = load_material_textures(material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

		std::vector<Texture> height_maps = load_material_textures(material, aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), height_maps.begin(), height_maps.end());
	}

	return Mesh{ vertices, indices, textures };
}

std::vector<Texture> Model::load_material_textures(aiMaterial* material, aiTextureType type, const std::string& type_name)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		bool skip = false;

		for (unsigned int j = 0; j < _textures_loaded.size(); j++)
		{
			if (std::strcmp(_textures_loaded[j]._path.data(), str.C_Str()) == 0)
			{
				textures.push_back(_textures_loaded[j]);
				skip = true;
				break;
			}
		}
		
		if (!skip)
		{
			Texture texture;
			texture._id = texture_from_file(str.C_Str(), _directory);
			texture._type = type_name;
			texture._path = str.C_Str();
			textures.push_back(texture);
			_textures_loaded.push_back(texture);
		}
	}

	return textures;
}

unsigned int texture_from_file(const std::string& path, const std::string& directory)
{
	std::string filename = path;
	filename = directory + '/' + filename;

	unsigned int texture_id;
	glGenTextures(1, &texture_id);

	int width, height, nr_components;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nr_components, 0);

	if (data)
	{
		GLenum format = 0;
		if (nr_components == 1)
		{
			format = GL_RED;
		}
		else if (nr_components == 3)
		{
			format = GL_RGB;
		}
		else if (nr_components == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cerr << "Error: Failed to load texture at path " << path << std::endl;
		stbi_image_free(data);
		exit(EXIT_FAILURE);
	}

	return texture_id;
}