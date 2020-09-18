#pragma once
#include "glm/glm.hpp"
#include "tools/shaderLoader.h"

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

		const vec3& GetSpecular() const ;

		virtual void SetLight(ShaderLoader& shader);

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

		void SetLight(ShaderLoader& shader) override;

	private:
		vec3 m_direction_;

	};

	class PointLight final : public BasicLight
	{
	public:
		PointLight() = default;

		~PointLight() = default;

		void SetLight(ShaderLoader& shader) override;

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

		void SetLight(ShaderLoader& shader) override;

	private:
		vec3 m_position_;
		vec3 m_direction_;
		float cutOff;

	};
}