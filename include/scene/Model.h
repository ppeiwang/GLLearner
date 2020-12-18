#pragma once
#include "RenderCoreDefine.h"
#include "logger/Logger.h"
#include "scene/Mesh.h"
#include "shader/Shader.h"
#include <string>

//namespace assimp
//{
	struct aiScene;
	struct aiNode;
	struct aiMesh;
//}

RENDER_CORE_BEGIN

class Model
{
public:
	Model(const std::string& str_model_path)
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

#if 0
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		const std::string& typeName);
#endif
		
private:
	std::vector<Mesh> vec_meshes_;

};

RENDER_CORE_END

