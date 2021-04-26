#include "scene/Model.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "utilities/Exception.h"
#include "tools/TextureLoader.h"
#include <filesystem>

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
	current_model_file_path_ = file_path;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		Logger::Error(std::string("Failed to load model from ") + file_path);
		return false;
	}

	if (!scene->mRootNode)
	{
		Logger::Error(std::string("Scene root node is null where loaded from file path: ") + file_path);
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
		Mesh render_core_mesh = ProcessMesh(mesh, ptr_scene);
		vec_meshes_.push_back(std::move(render_core_mesh));
	}
	// then do the same for each of its children
	for (int i = 0; i < ptr_node->mNumChildren; i++)
	{
		ProcessNode(ptr_node->mChildren[i], ptr_scene);
	}
}

Mesh Model::ProcessMesh(const aiMesh* ptr_mesh, const aiScene* ptr_scene)
{
	if (!ptr_mesh)
	{
		// todo
		// throw file and line num
		assert(false && "try to access an empty pointer");
		throw std::runtime_error{"ptr_mesh is null"};
	}

	if (!ptr_scene)
	{
		// todo
		// throw file and line num
		assert(false && "try to access an empty pointer");
		throw std::runtime_error{"ptr_scene is null"};
	}

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

		if (ptr_mesh->HasNormals())
		{
			vx.normal_ = { assimp_normal.x, assimp_normal.y, assimp_normal.z };
		}

		if (ptr_mesh->mTextureCoords[0])
		{
			const auto& assimp_texture_coord = ptr_mesh->mTextureCoords[0];
			vx.texture_coords_ = {assimp_texture_coord[i].x, assimp_texture_coord[i].y};
		}
		if (ptr_mesh->mTangents) {
			vx.tangent_ = { ptr_mesh->mTangents[i].x, ptr_mesh->mTangents[i].y, ptr_mesh->mTangents[i].z };
			vx.bitangent_ = { ptr_mesh->mBitangents[i].x, ptr_mesh->mBitangents[i].y, ptr_mesh->mBitangents[i].z };
		}

		vertices.push_back(std::move(vx));
	}
	// process indices. Triangle is the default polygon type
	for (unsigned int i = 0; i < ptr_mesh->mNumFaces; i++)
	{
		const aiFace& cref_face = ptr_mesh->mFaces[i];
		for (unsigned int j = 0; j < cref_face.mNumIndices; j++)
			indices.push_back(cref_face.mIndices[j]);
	}

	// process material
	if (ptr_mesh->mMaterialIndex >= 0)
	{
		const aiMaterial* material = ptr_scene->mMaterials[ptr_mesh->mMaterialIndex];
		const std::vector<Texture> diffuse_textures = LoadMaterialTextures(material, aiTextureType_DIFFUSE, ETextureType::texture_diffuse);
		textures.insert(textures.end(), diffuse_textures.begin(), diffuse_textures.end());
		const std::vector<Texture> specular_textures = LoadMaterialTextures(material, aiTextureType_SPECULAR, ETextureType::texture_specular);
		textures.insert(textures.end(), specular_textures.begin(), specular_textures.end());
	}

	return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}

std::vector<Texture> Model::LoadMaterialTextures(const aiMaterial* ptr_material, aiTextureType type, const ETextureType texture_type)
{
	if (!ptr_material)
	{
		assert(false && "try to access an empty pointer");
		throw std::runtime_error{ "ptr_material is null" };
	}

	std::vector<Texture> textures;
	for (unsigned int i = 0; i < ptr_material->GetTextureCount(type); i++)
	{
		aiString str;
		ptr_material->GetTexture(type, i, &str);

		std::filesystem::path current_path{current_model_file_path_};
	
		const std::string texture_full_path = current_path.parent_path().string() + "/" + str.C_Str();

		Texture texture = TextureLoader::LoadTexture(texture_full_path.c_str(), texture_type);

		textures.push_back(std::move(texture));
	}
	return textures;
}

RENDER_CORE_END


