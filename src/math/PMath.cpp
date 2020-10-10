#include "math/PMath.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec4;

RENDER_CORE_BEGIN

glm::mat4 PMath::TranslationMatrix(const glm::vec3& translation)
{
	return glm::translate(glm::mat4{ 1 }, translation);
}

glm::mat4 PMath::ScaleMatrix(const float scale)
{
	return glm::scale(glm::mat4{ 1 }, glm::vec3{scale});
}

glm::mat3 PMath::LookAtMatrix(const glm::vec3& z_axis, const glm::vec3& _up)
{
	const vec3 f(normalize(z_axis));
	const vec3 s(normalize(cross(_up, f)));
	const vec3 u(cross(f, s));
	return mat3{ vec3{s[0], s[1], s[2]}, vec3{u[0], u[1], u[2]}, vec3{f[0], f[1], f[2]} };
}

glm::mat4 PMath::MatrixRotateFromExtrinsicXYZ(float _ax, float _ay, float _az)
{
	return glm::eulerAngleZYX(_az, _ay, _ax);
}

glm::mat4 PMath::MatrixRotateFromExtrinsicZYX(float _ax, float _ay, float _az)
{
	return glm::eulerAngleXYZ(_ax, _ay, _az);
}

void PMath::NormalizeVector(const glm::vec3& vector, glm::vec3& out)
{
	if (glm::length(vector) < kEpsilon)
	{
		assert(false);
		out = vec3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		out = glm::normalize(vector);
	}
}

glm::quat PMath::VectorRotate(const glm::vec3& v0, const glm::vec3& v1)
{
	auto v0_n = glm::normalize(v0);
	auto v1_n = glm::normalize(v1);
	const auto dot = glm::dot(v0_n, v1_n);
	if (glm::epsilonEqual(dot, 1.0f, kEpsilon))
	{
		return glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f };
	}
	else if (glm::epsilonEqual(dot, -1.0f, kEpsilon)) // Take care of the collinear situation
	{
		if (glm::epsilonEqual(v1_n.x, 0.0f, kEpsilon))
		{
			v1_n.x = kEpsilon;
			v1_n = glm::normalize(v1_n);
		}
		else
		{
			v1_n.x = 0.0f;
			v1_n.y = kEpsilon;
			v1_n = glm::normalize(v1_n);
		}
	}

	const auto axis = glm::normalize(glm::cross(v0_n, v1_n));
	const auto theta = glm::acos(glm::clamp(glm::dot(v0_n, v1_n), -1.0f, 1.0f));

	return glm::quat{ cos(theta * 0.5f), axis * sin(theta * 0.5f) };
}

glm::quat PMath::QuaternionRotate(const float angleRad, const glm::vec3& v)
{
	const float Sin = sin(angleRad * 0.5f);

	return glm::quat(cos(angleRad * 0.5f), v.x * Sin, v.y * Sin, v.z * Sin);
}

#define EXTRACT_TRANSLATION_PART \
	/* extract translation */	\
	translation.x = t[3][0];	\
	translation.y = t[3][1];	\
	translation.z = t[3][2];	\
	do{} while(false)

#define EXTRACT_SCALE_PART \
	/* extract the columns of the matrix. */ \
	glm::mat3 m = { \
		glm::vec3{t[0][0], t[0][1], t[0][2]},\
		glm::vec3{t[1][0], t[1][1], t[1][2]},\
		glm::vec3{t[2][0], t[2][1], t[2][2]} \
	}; \
	/* extract the scaling factors */	\
	scale.x = glm::length(m[0]); \
	scale.y = glm::length(m[1]); \
	scale.z = glm::length(m[2]); \
	/* and the sign of the scaling */ \
	if (glm::determinant(t) < 0)\
	{\
		scale = -scale; \
	}\
	/* and remove all scaling from the matrix */ \
	if (scale.x) m[0] /= scale.x; \
	if (scale.y) m[1] /= scale.y; \
	if (scale.z) m[2] /= scale.z; \
	do {} while (false)

void PMath::TransformMatrixDecompose(const glm::mat4& t, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
{
	EXTRACT_TRANSLATION_PART;
	EXTRACT_SCALE_PART;
	/*
		assuming a right-handed coordinate system
		and post-multiplication of column vectors,
		the rotation matrix for an euler XYZ rotation is M = Rz * Ry * Rx.
		combining gives:

			|  CE  BDE-AF  ADE+BF  0  |
		M = |  CF  BDF+AE  ADF-BE  0  |
			|  -D    CB      AC    0  |
			|   0     0       0    1  |

		where
		A = cos(angle_x), B = sin(angle_x);
		C = cos(angle_y), D = sin(angle_y);
		E = cos(angle_z), F = sin(angle_z);
	*/

	rotation.y = std::asin(-m[0].z);// D. Angle around Y-axis.

	const auto C = std::cos(rotation.y);

	if (std::fabs(C) > glm::epsilon<float>())
	{
		// Finding angle around X-axis.
		auto tan_x = m[2].z / C;	// A
		auto tan_y = m[1].z / C;	// B

		rotation.x = std::atan2(tan_y, tan_x);
		// Finding angle around Z-axis.
		tan_x = m[0].x / C;	// E
		tan_y = m[0].y / C;	// F
		rotation.z = std::atan2(tan_y, tan_x);
	}
	else
	{
		// Y-axis is fixed.
		rotation.x = 0;// Set angle around oX to 0. => A == 1, B == 0, C == 0, D == 1.

		// And finding angle around Z-axis.
		const auto tan_x = m[1].y;		// BDF+AE => E
		const auto tan_y = -m[1].x;		// BDE-AF => F

		rotation.z = std::atan2(tan_y, tan_x);
	}
}

void PMath::TransformMatrixDecompose(const glm::mat4& t, glm::vec3& translation, glm::quat& q, glm::vec3& scale)
{
	EXTRACT_TRANSLATION_PART;
	EXTRACT_SCALE_PART;
	q = glm::quat{ m };
}

glm::vec3 PMath::ExtractTranslation(const glm::mat4& t)
{
	return glm::vec3{ t[3][0], t[3][1], t[3][2] };
}

glm::vec3 PMath::ExtractScale(const glm::mat4& t)
{
	glm::vec3 scale;
	EXTRACT_SCALE_PART;
	return scale;
}

RENDER_CORE_END
