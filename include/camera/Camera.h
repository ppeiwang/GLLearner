#pragma once
#include "RenderCoreDefine.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

RENDER_CORE_BEGIN

class Camera
{
public:
	Camera();

	void Update();

	void Reset();

	void SetTarget(const glm::vec3& target);

	void SetPosition(const glm::vec3& pos);

	void SetRotation(const glm::quat& q);

	void SetUpDirection(const glm::vec3& up);

	void SetPerspective(float fov, float asp, float zNear, float zFar);

	void Pitch(float angleRad);

	void Yaw(float angleRad);

	void Roll(float angleRad);

	void Rotate(const float angleRad, const glm::vec3& axis);

	float GetNear() const noexcept { return m_zNear_; }

	float GetFar() const noexcept { return m_zFar_; }

	float GetFov() const noexcept { return m_fov_; }

	float GetAsp() const noexcept { return m_asp_; }

	const glm::vec3& GetTarget() const noexcept;

	// world position
	const glm::vec3& GetPosition() const noexcept;

	const glm::quat& GetRotation() const noexcept;

	const glm::vec3& GetEuler() const noexcept;

	const glm::vec3& GetUpDirection() const noexcept;

	glm::vec3 GetForward() const noexcept;

	/**
	* @brief Get the camera's right direction (world coordinate system)
	*/
	glm::vec3 GetRight() const noexcept;

	/**
	* @brief  Get the camera's up direction (world coordinate system)
	* Actually, this function is not equivalent to "GetUpDirection" which returns the vector constructing the camera's local coordinate system
	*/
	glm::vec3 GetUp() const noexcept;

	const glm::mat3& GetRotationMatrix() const noexcept;

	const glm::mat4& GetViewMatrix() const noexcept;

	const glm::mat4& GetProjectMatrix() const noexcept;

	glm::quat last_camera_rotation = { 1.0f, 0.f, 0.f, 0.f };

	glm::vec3 last_camera_position = { 0.f, 0.f, 0.f };

	glm::vec3 camera_displacement = { 0.f, 0.f, 0.f };

private:
	void UpdateCamera();

private:
	bool m_dirty_;
	
	bool m_free_camera_;

	float m_fov_;

	float m_asp_;
	
	float m_zNear_;

	float m_zFar_;

	glm::vec3 m_position_;

	glm::vec3 m_target_;

	glm::vec3 m_up_direction_;

	glm::vec3 m_raw_view_direction_; // the camera's default view direction

	glm::quat m_rotation_;

	glm::vec3 m_rotation_euler_;

	glm::mat3 m_rotation_matrix_;

	glm::mat4 m_view_matrix_;

	glm::mat4 m_projection_matrix_;

};

RENDER_CORE_END
