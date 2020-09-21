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

	void DirectionLight::SetDirection(const vec3& v3)
	{
		m_direction_ = v3;
	}

	const vec3& DirectionLight::GetDirection() const
	{
		return m_direction_;
	}

	void PointLight::SetPosition(const vec3& pos)
	{
		m_position_ = pos;
	}

	const vec3& PointLight::GetPosition() const
	{
		return m_position_;
	}

	float PointLight::GetConstant() const
	{
		return m_constant_;
	}

	void PointLight::SetConstant(float v)
	{
		m_constant_ = v;
	}

	float PointLight::GetLinear() const
	{
		return m_linear_;
	}

	void PointLight::SetLinear(float v)
	{
		m_linear_ = v;
	}

	float PointLight::GetQuadratic() const
	{
		return m_quadratic_;
	}

	void PointLight::SetQuadratic(float v)
	{
		m_quadratic_ = v;
	}

	void SpotLight::SetDirection(const vec3& v3)
	{
		m_direction_ = v3;
	}

	void SpotLight::SetPosition(const vec3& v3)
	{
		m_position_ = v3;
	}

	const vec3& SpotLight::GetDirection() const
	{
		return m_direction_;
	}

	const vec3& SpotLight::GetPosition() const
	{
		return m_position_;
	}

	float SpotLight::GetCutOff() const
	{
		return m_cut_off_;
	}

	void SpotLight::SetCutOff(float v)
	{
		m_cut_off_ = v;
	}

	void SpotLight::SetOuterCutOff(float v)
	{
		m_out_cut_off_ = v;
	}

	float SpotLight::GetOuterCutOff() const
	{
		return m_out_cut_off_;
	}

}


