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
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		Logger::Error(std::string("Failed to load model from ") + file_path);
		return false;
	}

	if (!scene->mRootNode)
	{
		Logger::Error(std::string("Scene root node is null from file path: ") + file_path);
		return;
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
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < ptr_mesh->mNumVertices; i++)
	{
		Vertex vx;
		// process vertex positions, normals and texture coordinates
		const auto& assimp_vertex = ptr_mesh->mVertices[i];
		const auto& assimp_normal = ptr_mesh->mNormals[i];
		vx.position_ = { assimp_vertex.x, assimp_vertex.y, assimp_vertex.z };
		vx.normal_ = { assimp_normal.x, assimp_normal.y, assimp_normal.z };
		if (ptr_mesh->mTextureCoords[0])
		{
			const auto& assimp_texture_coord = ptr_mesh->mTextureCoords[0];
			vx.texture_coords_ = {assimp_texture_coord[i].x, assimp_texture_coord[i].y};
		}
		vx.texture_coords_ = {};
		vertices.push_back(std::move(vx));
	}
	// process indices
	[...]
	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		[...]
	}

	return Mesh(vertices, indices, textures);

	// todo: remove this
	return {};
}

RENDER_CORE_END


