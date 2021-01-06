#pragma once
#include "RenderCoreDefine.h"
#include "Vertex.h"
#include "Texture.h"
#include "shader/Shader.h"
#include <vector>

RENDER_CORE_BEGIN

class Mesh
{
public:
	Mesh() = default;

	~Mesh() = default;

	template<typename T1, typename T2, typename T3>
	Mesh(T1&& vertices, T2&& indices, T3&& textures) :
		m_vertices_{std::forward<T1>(vertices)},
		m_indices_{std::forward<T2>(indices)},
		m_textures_{std::forward<T3>(textures)},
		m_vertex_array_object_ {-1},
		m_vertex_buffer_object_ {-1},
		m_element_buffer_object_ {-1}
	{

	}

	inline std::vector<Vertex>& GetVertices();

	inline const std::vector<Vertex>& GetVertices() const;

	inline std::vector<uint32_t>& GetIndices();

	inline const std::vector<uint32_t>& GetIndices() const;

	inline std::vector<Texture>& GetTextures();

	inline const std::vector<Texture>& GetTextures() const;

	void Draw(Shader& shader) const ;

	void SetupMesh();

private:
	std::vector<Vertex> m_vertices_;
	std::vector<uint32_t> m_indices_;
	std::vector<Texture> m_textures_;

	uint32_t m_vertex_array_object_;
	uint32_t m_vertex_buffer_object_;
	uint32_t m_element_buffer_object_;
};

RENDER_CORE_END
