#include "light/BasicLight.h"

namespace Light
{
	using glm::vec3;

	void BasicLight::SetAmbient(const vec3& v3) 
	{
		m_ambient_ = v3;
	}

	void BasicLight::SetDiffuse(const vec3& v3)
	{
		m_diffuse_ = v3;
	}

	void BasicLight::SetSpecular(const vec3& v3)
	{
		m_specular_ = v3;
	}

	const vec3& BasicLight::GetAmbient() const
	{
		return m_ambient_;
	}

	const vec3& BasicLight::GetDiffuse() const
	{
		return m_diffuse_;
	}

	const vec3& BasicLight::GetSpecular() const
	{
		return m_specular_;
	}

	void BasicLight::SetLight(ShaderLoader& shader)
	{
		
	}
}


