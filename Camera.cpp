#include "Camera.h"
#include "PMath.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera()
	: dirty_(true)
	, free_camera_(true)
	, fov_(0.f)
	, asp_(0.f)
	, zFar_(0.f)
	, zNear_(0.f)
	, position_{0.f, 0.f, 0.f}
	, rotation_{ 1.0f, 0.f, 0.f, 0.f }
	, target_{0.f, 0.f, -1.f}
	, up_direction_{ 0.f, 1.f, 0.f }
	, raw_view_direction_{0.f, 0.f, -1.f}
	, rotationMatrix_{1.f}
	, viewMatrix_{1.f}
	, projMatrix_{1.f}
{

}

void Camera::UpdateCamera()
{
	// rotate view direction
	rotationMatrix_ = glm::mat3{rotation_};

	glm::vec3 direction = rotationMatrix_ * raw_view_direction_;
	glm::vec3 new_target{ position_.x + direction.x, position_.y + direction.y, position_.z + direction.z };

	// rotate up direction
	glm::vec3 new_up_direction = rotationMatrix_ * up_direction_ ;

	viewMatrix_ = glm::lookAtRH(position_, new_target, up_direction_);
	projMatrix_ = glm::perspective(fov_, asp_, zNear_, zFar_);
}

void Camera::Update()
{
	if (dirty_)
	{
		UpdateCamera();
		dirty_ = false;
	}
}

void Camera::SetTarget(const glm::vec3& target)
{
	target_ = target_;
	dirty_ = true;
}

void Camera::SetPosition(const glm::vec3& pos)
{
	position_ = pos;
	dirty_ = true;
}

void Camera::SetUpDirection(const glm::vec3& up)
{
	up_direction_ = up;
	dirty_ = true;
}

void Camera::SetPerspective(float fov, float asp, float zNear, float zFar)
{
	fov_ = fov;
	asp_ = asp;
	zNear_ = zNear;
	zFar_ = zFar;
	dirty_ = true;
}

void Camera::Pitch(float angleRad)
{
	const auto& x_axis = rotationMatrix_[0];// rotate around the current x axis
	Rotate(angleRad, x_axis);
}

void Camera::Yaw(float angleRad)
{
	if (free_camera_)
	{
		Rotate(angleRad, glm::vec3{ 0.f, 1.f, 0.f }); // always around with the y axis
	}
	else
	{
		const auto& y_axis = rotationMatrix_[1];// rotate around the current y axis
		Rotate(angleRad, y_axis );
	}
}

void Camera::Roll(float angleRad)
{
	const auto& z_axis = rotationMatrix_[2];// rotate around the current z axis
	Rotate(angleRad, z_axis );
}

void Camera::Rotate(const float angleRad, const glm::vec3& axis)
{
	const glm::quat rot_q = PMath::QuaternionRotate(angleRad, axis);
	const glm::quat& current_rot_q = GetRotation();
	const glm::quat rot_result = glm::normalize(rot_q*current_rot_q);
	rotation_ = rot_result;
	dirty_ = true;
}

float Camera::GetNear() const
{
	return zNear_;
}

float Camera::GetFar() const
{
	return zFar_;
}

const glm::vec3& Camera::GetTarget() const noexcept
{
	return target_;
}

const glm::vec3& Camera::GetPosition() const noexcept
{
	return position_;
}

const glm::quat& Camera::GetRotation() const noexcept
{
	return rotation_;
}

const glm::vec3& Camera::GetUpDirection() const noexcept
{
	return up_direction_;
}

glm::vec3 Camera::GetForward() const noexcept
{
	assert(false);
	return glm::vec3{};
}

glm::vec3 Camera::GetRight() const noexcept
{
	assert(false);
	return glm::vec3{};
}

glm::vec3 Camera::GetUp() const noexcept
{
	assert(false);
	return glm::vec3{};
}

const glm::mat4& Camera::GetViewMatrix() const noexcept
{
	return viewMatrix_;
}

const glm::mat4& Camera::GetProjectMatrix() const noexcept
{
	return projMatrix_;
}