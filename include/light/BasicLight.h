#pragma once
#include "RenderCoreDefine.h"
#include "glm/glm.hpp"

RENDER_CORE_BEGIN

namespace Light
{
	enum class LigthType
	{
		None = 0,
		DirectionLight = 1,
		PointLight = 2,
		SpotLight = 3
	};

	class BasicLight
	{
	protected:
		using vec3 = glm::vec3;

	public:
		BasicLight() = default;

		virtual ~BasicLight() = default;

		void SetAmbient(const vec3& v3);

		void SetDiffuse(const vec3& v3);

		void SetSpecular(const vec3& v3);

		const vec3& GetAmbient() const;

		const vec3& GetDiffuse() const;

		const vec3& GetSpecular() const;

	protected:
		vec3 m_ambient_;
		vec3 m_diffuse_;
		vec3 m_specular_;
	};

	class DirectionLight final : public BasicLight
	{
	public:
		DirectionLight() = default;

		~DirectionLight() = default; // override basic light

		void SetDirection(const vec3& v3);

		const vec3& GetDirection() const;

	private:
		vec3 m_direction_;
	};

	class PointLight final : public BasicLight
	{
	public:
		PointLight() = default;

		~PointLight() = default;

		void SetPosition(const vec3& pos);

		void SetConstant(float v);

		void SetLinear(float v);

		void SetQuadratic(float v);

		const vec3& GetPosition() const;

		float GetConstant() const;

		float GetLinear() const;

		float GetQuadratic() const;

	private:
		vec3 m_position_;
		float m_constant_;
		float m_linear_;
		float m_quadratic_;
	};

	class SpotLight final : public BasicLight
	{
	public:
		SpotLight() = default;

		~SpotLight() = default;

		void SetDirection(const vec3& v3);

		void SetPosition(const vec3& v3);

		void SetCutOff(float v);

		void SetOuterCutOff(float v);

		const vec3& GetDirection() const;

		const vec3& GetPosition() const;

		float GetCutOff() const;

		float GetOuterCutOff() const;

	private:
		vec3 m_position_;
		vec3 m_direction_;
		float m_cut_off_;
		float m_out_cut_off_;
	};
}

RENDER_CORE_END