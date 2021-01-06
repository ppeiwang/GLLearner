#pragma once
#include "RenderCoreDefine.h"
#include "logger/Logger.h"
#include "scene/Mesh.h"
#include "shader/Shader.h"
#include <string>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

RENDER_CORE_BEGIN

class Model
{
public:
	explicit Model(const std::string& str_model_path)
	{
		const bool load_success = LoadModel(str_model_path);
		if (!load_success)
		{
			Logger::Warning(std::string("Failed to load model from: ") + str_model_path);
		}
	}

	~Model() = default;

	Model(const Model&) = delete;

	Model& operator = (const Model&) = delete;

	void Draw(Shader& shader);

private:
	bool LoadModel(const std::string& file_path);

	void ProcessNode(const aiNode* ptr_node, const aiScene* ptr_scene);

	Mesh ProcessMesh(const aiMesh* ptr_mesh, const aiScene* ptr_scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const ETextureType textue_type);
		
private:
	std::vector<Mesh> vec_meshes_;

	std::string current_model_file_path_;

};

RENDER_CORE_END

