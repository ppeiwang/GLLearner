#include "camera/Camera.h"
#include "math/PMath.h"
#include "glm/gtc/matrix_transform.hpp"
#include "logger/Logger.h"
#include "glm/gtc/epsilon.hpp"

using glm::vec3;
using glm::quat;

namespace
{
const vec3 k_default_camera_position = { 0.f, 0.f, 0.f };
const quat k_default_camera_rotation = { 1.0f, 0.f, 0.f, 0.f };
const vec3 k_default_camera_rotation_euler = { 0.f, 0.f, 0.f };
const vec3 k_default_camera_target = { 0.f, 0.f, -1.f };
const vec3 k_default_camera_up_dir = { 0.f, 1.f, 0.f };
const vec3 k_default_camera_raw_view_dir = { 0.f, 0.f, -1.f };
}

RENDER_CORE_BEGIN

Camera::Camera()
	: m_dirty_(true)
	, m_free_camera_(true)
	, m_fov_(0.f)
	, m_asp_(0.f)
	, m_zFar_(0.f)
	, m_zNear_(0.f)
	, m_position_(k_default_camera_position)
	, m_rotation_(k_default_camera_rotation)
	, m_rotation_euler_(k_default_camera_rotation_euler)
	, m_target_(k_default_camera_target)
	, m_up_direction_(k_default_camera_up_dir)
	, m_raw_view_direction_(k_default_camera_raw_view_dir)
	, m_rotation_matrix_{ 1.f }
	, m_view_matrix_{ 1.f }
	, m_projection_matrix_{ 1.f }
{
}

void Camera::UpdateCamera()
{
	// rotate view direction
	m_rotation_matrix_ = glm::mat3{ m_rotation_ };

	glm::vec3 direction = m_rotation_matrix_ * m_raw_view_direction_;
	glm::vec3 new_target{ m_position_.x + direction.x, m_position_.y + direction.y, m_position_.z + direction.z };

	m_target_ = new_target;

	// rotate up direction
	glm::vec3 new_up_direction = m_rotation_matrix_ * m_up_direction_;

	m_view_matrix_ = glm::lookAtRH(m_position_, new_target, m_up_direction_);
	m_projection_matrix_ = glm::perspective(m_fov_, m_asp_, m_zNear_, m_zFar_);
}

void Camera::Update()
{
	if (m_dirty_)
	{
		UpdateCamera();
		m_dirty_ = false;
	}
}

void Camera::Reset()
{
	m_position_ = k_default_camera_position;
	m_rotation_ = k_default_camera_rotation;
	m_target_ = k_default_camera_target;
	m_up_direction_ = k_default_camera_up_dir;
	m_raw_view_direction_ = k_default_camera_raw_view_dir;
	m_rotation_euler_ = k_default_camera_rotation_euler;

	camera_displacement = k_default_camera_position;
	last_camera_position = k_default_camera_position;

	m_dirty_ = true;
}

void Camera::SetTarget(const glm::vec3& target)
{
	auto concurrent = glm::epsilonNotEqual(target, m_position_, glm::epsilon<float>());
	if (concurrent.x && concurrent.y && concurrent.z)
	{
		Logger::Warning("Camera Can not set the target and target at the same point");
		return;
	}

	const auto view_direction = glm::normalize(target - m_position_);

	const auto quat_to_new_view = PMath::VectorRotate(m_raw_view_direction_, view_direction);

	SetRotation(quat_to_new_view);
}

void Camera::SetPosition(const glm::vec3& pos)
{
	if (m_position_ != pos)
	{
		last_camera_position = pos;

		camera_displacement = glm::normalize(m_position_ - last_camera_position);

		m_position_ = pos;
		m_dirty_ = true;
	}
}

void Camera::SetRotation(const glm::quat& q)
{
	m_dirty_ = true;
	m_rotation_ = q;
}

void Camera::SetUpDirection(const glm::vec3& up)
{
	m_up_direction_ = up;
	m_dirty_ = true;
}

void Camera::SetPerspective(float fov, float asp, float zNear, float zFar)
{
	if (m_fov_ != fov || m_asp_ != asp || m_zNear_ != zNear || m_zFar_ != zFar)
	{
		m_fov_ = fov;
		m_asp_ = asp;
		m_zNear_ = zNear;
		m_zFar_ = zFar;
		m_dirty_ = true;
	}
}

void Camera::Pitch(float angleRad)
{
	m_rotation_euler_.x += angleRad;
	const auto& x_axis = m_rotation_matrix_[0]; // rotate around the current x axis
	Rotate(angleRad, x_axis);
}

void Camera::Yaw(float angleRad)
{
	m_rotation_euler_.y += angleRad;

	if (m_free_camera_)
	{
		Rotate(angleRad, glm::vec3{ 0.f, 1.f, 0.f }); // always around with the y axis
	}
	else
	{
		const auto& y_axis = m_rotation_matrix_[1]; // rotate around the current y axis
		Rotate(angleRad, y_axis);
	}
}

void Camera::Roll(float angleRad)
{
	m_rotation_euler_.z += angleRad;

	const auto& z_axis = m_rotation_matrix_[2]; // rotate around the current z axis
	Rotate(angleRad, z_axis);
}

void Camera::Rotate(const float angleRad, const glm::vec3& axis)
{
	const glm::quat rot_q = PMath::QuaternionRotate(angleRad, axis);
	const glm::quat& current_rot_q = GetRotation();
	const glm::quat& tmp = rot_q * current_rot_q;
	const glm::quat rot_result = glm::normalize(rot_q * current_rot_q);

	m_dirty_ = true;
	last_camera_rotation = m_rotation_;
	m_rotation_ = rot_result;
}

const glm::vec3& Camera::GetTarget() const noexcept
{
	return m_target_;
}

const glm::vec3& Camera::GetPosition() const noexcept
{
	return m_position_;
}

const glm::quat& Camera::GetRotation() const noexcept
{
	return m_rotation_;
}

const glm::vec3& Camera::GetEuler() const noexcept
{
	return m_rotation_euler_;
}

const glm::vec3& Camera::GetUpDirection() const noexcept
{
	return m_up_direction_;
}

glm::vec3 Camera::GetForward() const noexcept
{
	return glm::vec3{ -m_rotation_matrix_[2] };
}

glm::vec3 Camera::GetRight() const noexcept
{
	return glm::vec3{ m_rotation_matrix_[0] };
}

glm::vec3 Camera::GetUp() const noexcept
{
	return glm::vec3{ m_rotation_matrix_[1] };
}

const glm::mat3& Camera::GetRotationMatrix() const noexcept
{
	return m_rotation_matrix_;
}

const glm::mat4& Camera::GetViewMatrix() const noexcept
{
	return m_view_matrix_;
}

const glm::mat4& Camera::GetProjectMatrix() const noexcept
{
	return m_projection_matrix_;
}

RENDER_CORE_END