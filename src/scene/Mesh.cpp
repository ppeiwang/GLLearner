#include "scene/Mesh.h"
#include "glad/glad.h"
#include "logger/Logger.h"
#include "Config.h"

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

void Mesh::SetupMesh()
{
	if (m_vertices_.size() > 0 && m_indices_.size() > 0)
	{
		glGenVertexArrays(1, &m_vertex_array_object_);
		glGenBuffers(1, &m_vertex_buffer_object_);
		glGenBuffers(1, &m_element_buffer_object_);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), then configure vertex attributes(s), and at last bind element buffer.

		glBindVertexArray(m_vertex_array_object_);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_object_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices_.size(), &m_vertices_[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_object_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices_[0]) * m_indices_.size(), &m_indices_[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal_));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords_));

		// unbind VBO and VAO
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	else
	{
		Logger::Warning("Bad Mesh Data. vertex or index array is empty");
	}

}

std::string operator + (const std::string& str, const std::string_view& strv)
{
	std::string s{ str };
	s.append(strv);
	return s;
}

void Mesh::Draw(Shader& shader) const
{
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;

	for (int i = 0; i < m_textures_.size(); i++)
	{
		if (m_textures_[i].type_ == ETextureType::texture_diffuse) {
			if (diffuseNr >= global::k_texture_diffuse_limit) {
				Logger::Error(std::string("Texture diffuse count is out of the limit: ") + std::to_string(global::k_texture_diffuse_limit));
				continue;
			}
		}
		else if (m_textures_[i].type_ == ETextureType::texture_specular) {
			if (specularNr >= global::k_texture_specular_limit) {
				Logger::Error(std::string("Texture diffuse count is out of the limit: ") + std::to_string(global::k_texture_specular_limit));
				continue;
			}
		}
		else {
			assert(false && "unknown if-else branch");
			continue;
		}

		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string material_name = "material.";
		std::string number;
		const std::string_view& name = Texture::TextureTypeToString(m_textures_[i].type_);
		if (m_textures_[i].type_ == RenderCore::ETextureType::texture_diffuse)
		{
			number = std::to_string(diffuseNr++);
		}
		else if (m_textures_[i].type_ == RenderCore::ETextureType::texture_specular)
		{
			number = std::to_string(specularNr++);
		}

		//shader.SetInt((std::string(name) + number).c_str(), i);
		shader.SetInt((material_name + name).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_textures_[i].id_);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(m_vertex_array_object_);
	glDrawElements(GL_TRIANGLES, m_indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

RENDER_CORE_END


