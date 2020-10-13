#pragma once
#include "RenderCoreDefine.h"
#include "light/BasicLight.h"
#include <string>

RENDER_CORE_BEGIN

class Shader
{
public:
	explicit Shader(unsigned int ID) : ID_{ ID }
	{

	}

	Shader(unsigned int ID, const std::string& str_vs, const std::string& str_fs)
		: ID_{ ID }
		, m_vs_path_ {str_vs}
		, m_fs_path_ {str_fs}
	{

	}

	void SetShaderInfo(const std::string& str_vs, const std::string& str_fs);

	// use/activate the shader
	void Use();

	// utility uniform functions
	void SetBool(const std::string& name, bool value) const;

	void SetInt(const std::string& name, int value) const;

	void SetFloat(const std::string& name, float value) const;

	void SetFloatVec(const std::string& name, const glm::vec3& vec) const;

	void SetFloatVec(const std::string& name, const glm::vec4& vec) const;

	void SetFloatVec(const std::string& name, float f0, float f1, float f2) const;

	void SetFloatVec(const std::string& name, float f0, float f1, float f2, float f3) const;

	void SetMatrix(const std::string& name, const glm::mat4& mat) const;

	void GetBool(const std::string& name, bool& value) const;

	void GetInt(const std::string& name, int& value) const;

	void GetFloat(const std::string& name, float& value) const;

	bool AddDirectionLight(const Light::DirectionLight& d_light);

	bool AddPointLight(const Light::PointLight& p_light);

	bool AddSpotLight(const Light::SpotLight& s_light);

	void ResetLightCount();

public:
	const unsigned int ID_;

	unsigned int m_direction_light_count{ 0 };

	unsigned int m_point_light_count{ 0 };

	unsigned int m_spot_light_count{ 0 };

	std::string m_vs_path_;

	std::string m_fs_path_;

};


RENDER_CORE_END

