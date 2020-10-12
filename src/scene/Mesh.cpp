#include "scene/Mesh.h"

RENDER_CORE_BEGIN

inline std::vector<Vertex>& Mesh::GetVertices()
{
	return m_vertices_;
}

inline const std::vector<Vertex>& Mesh::GetVertices() const
{
	return m_vertices_;
}

inline std::vector<uint32_t>& Mesh::GetIndices()
{
	return m_indices_;
}

inline const std::vector<uint32_t>& Mesh::GetIndices() const
{
	return m_indices_;
}

inline std::vector<Texture>& Mesh::GetTextures()
{
	return m_textures_;
}

inline const std::vector<Texture>& Mesh::GetTextures() const
{
	return m_textures_;
}

RENDER_CORE_END

