#include "scene/Model.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

RENDER_CORE_BEGIN

void Model::Draw(Shader& shader)
{
	for (const auto& mesh : vec_meshes_)
	{
		mesh.Draw(shader);
	}
}

bool Model::LoadModel(const std::string& file_path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Logger::Error(std::string("Failed to load model from ") + file_path);
		return false;
	}

	ProcessNode(scene->mRootNode, scene);

	return true;
}

void Model::ProcessNode(const aiNode* ptr_node, const aiScene* ptr_scene)
{
	// process all the node's meshes (if any)
	for (int i = 0; i < ptr_node->mNumMeshes; i++)
	{
		aiMesh* mesh = ptr_scene->mMeshes[ptr_node->mMeshes[i]];
		vec_meshes_.push_back(ProcessMesh(mesh, ptr_scene));
	}
	// then do the same for each of its children
	for (int i = 0; i < ptr_node->mNumChildren; i++)
	{
		ProcessNode(ptr_node->mChildren[i], ptr_scene);
	}
}

Mesh Model::ProcessMesh(const aiMesh* ptr_mesh, const aiScene* ptr_scene)
{

	// todo: remove this
	return {};
}

RENDER_CORE_END


