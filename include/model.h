#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "shader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <string>
#include <vector>

class Model
{
public:
	Model(const std::string& path);
	void draw(Shader& shader);

private:
	std::vector<Mesh> _meshes;
	std::vector<Texture> _textures_loaded;
	std::string _directory;

	void load_model(const std::string& path);
	void process_node(aiNode *mesh, const aiScene* scene);
	Mesh process_mesh(aiMesh *mesh, const aiScene* scene);
	std::vector<Texture> load_material_textures(aiMaterial* material, aiTextureType type, const std::string& type_name);
};

#endif