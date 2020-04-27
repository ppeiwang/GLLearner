#include "GLM_Transform.h"
#include <glm/gtc/matrix_transform.hpp>


glm::mat4x4 glm_base::MatrixTranslate(float _tx, float _ty, float _tz)
{
	return glm::translate(glm::mat4x4(1), glm::vec3(_tx, _ty, _tz));
}

void glm_base::MatrixTranspose(glm::mat4x4& _result, const glm::mat4x4& _a)
{
	_result = glm::transpose(_a);
}

void glm_base::MatrixScale(glm::mat4x4& _result, float _sx, float _sy, float _sz)
{
	_result = glm::mat4x4{ 1 };
	_result = glm::scale(_result, glm::vec3(_sx, _sy, _sz));
}

void glm_base::MatrixTranslate(glm::mat4x4& _result, float _tx, float _ty, float _tz)
{
	_result = glm::mat4x4{ 1 };
	_result = glm::translate(_result, glm::vec3(_tx, _ty, _tz));
}

glm::mat4x4 glm_base::MatrixScale(float _sx, float _sy, float _sz)
{
	return glm::scale(glm::mat4x4(1), glm::vec3(_sx, _sy, _sz));
}

void glm_base::MatrixRotateXYZ(glm::mat4x4& _result, float _ax, float _ay, float _az)
{
	_result = MatrixRotateXYZ(_ax, _ay, _az);
}

glm::mat4x4 glm_base::MatrixIdentity()
{
	return glm::mat4x4{ 1 };
}

glm::mat4x4 glm_base::MatrixLookAt(const glm::vec3& _eye, const glm::vec3& _at, const glm::vec3& _up)
{
	return glm::lookAt(_eye, _at, _up);
}

glm::mat4x4 glm_base::MatrixRotateXYZ(float _ax, float _ay, float _az)
{
	const float sx = sin(_ax);
	const float cx = cos(_ax);
	const float sy = sin(_ay);
	const float cy = cos(_ay);
	const float sz = sin(_az);
	const float cz = cos(_az);

	glm::mat4x4 _result{ 1 };
	_result[0][0] = cz * cy;
	_result[0][1] = cz * sy * sx + sz * cx;
	_result[0][2] = sz * sx - cz * sy * cx;

	_result[1][0] = -sz * cy;
	_result[1][1] = cz * cx - sz * sy * sx;
	_result[1][2] = sz * sy * cx + cz * sx;

	_result[2][0] = sy;
	_result[2][1] = -cy * sx;
	_result[2][2] = cy * cx;

	return _result;
}

glm::mat4x4 glm_base::MatrixRotateZYX(float _ax, float _ay, float _az)
{
	const float sx = sin(_ax);
	const float cx = cos(_ax);
	const float sy = sin(_ay);
	const float cy = cos(_ay);
	const float sz = sin(_az);
	const float cz = cos(_az);

	glm::mat4x4 _result{ 1 };

	_result[0][0] = cz * cy;
	_result[0][1] = sz * cy;
	_result[0][2] = -sy;

	_result[1][0] = cz * sy * sx - sz * cx;;
	_result[1][1] = cz * cx + sz * sy * sx;
	_result[1][2] = cy * sx;

	_result[2][0] = sz * sx + cz * sy * cx;
	_result[2][1] = sz * sy * cx - sx * cz;
	_result[2][2] = cy * cx;

	return _result;
}

glm::mat4x4 glm_base::MatrixRotateX(float _ax)
{
	const auto c = glm::cos(_ax);
	const auto s = glm::sin(_ax);
	glm::mat4x4 m{ 1 };
	m[1][1] = c;
	m[1][2] = s;
	m[2][1] = -s;
	m[2][2] = c;
	return m;
}

glm::mat4x4 glm_base::MatrixRotateY(float _ay)
{
	const auto c = glm::cos(_ay);
	const auto s = glm::sin(_ay);
	glm::mat4x4 m{ 1 };
	m[0][0] = c;
	m[0][2] = -s;
	m[2][0] = s;
	m[2][2] = c;
	return m;
}

glm::mat4x4 glm_base::MatrixRotateZ(float _az)
{
	const auto c = glm::cos(_az);
	const auto s = glm::sin(_az);
	glm::mat4x4 m{ 1 };
	m[0][0] = c;
	m[0][1] = s;
	m[1][0] = -s;
	m[1][1] = c;
	return m;
}

void glm_base::MatrixDecompose(const glm::mat4x4& t, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
{
	/* extract translation */
	translation.x = t[3][0];
	translation.y = t[3][1];
	translation.z = t[3][2];

	/* extract the columns of the matrix. */
	glm::mat3 m = {
		glm::vec3{t[0][0], t[0][1], t[0][2]},
		glm::vec3{t[1][0], t[1][1], t[1][2]},
		glm::vec3{t[2][0], t[2][1], t[2][2]}
	};

	/* extract the scaling factors */
	scale.x = glm::length(m[0]);
	scale.y = glm::length(m[1]);
	scale.z = glm::length(m[2]);

	/* and the sign of the scaling */
	if (glm::determinant(t) < 0)
	{
		scale = -scale;
	}

	/* and remove all scaling from the matrix */
	if (scale.x) m[0] /= scale.x;
	if (scale.y) m[1] /= scale.y;
	if (scale.z) m[2] /= scale.z;

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

void glm_base::MatrixDecompose(const glm::mat4x4& t, glm::vec3& translation, glm::quat& q, glm::vec3& scale)
{
	/* extract translation */
	translation.x = t[3][0];
	translation.y = t[3][1];
	translation.z = t[3][2];

	/* extract the columns of the matrix. */
	glm::mat3 m = {
		glm::vec3{t[0][0], t[0][1], t[0][2]},
		glm::vec3{t[1][0], t[1][1], t[1][2]},
		glm::vec3{t[2][0], t[2][1], t[2][2]}
	};

	/* extract the scaling factors */
	scale.x = glm::length(m[0]);
	scale.y = glm::length(m[1]);
	scale.z = glm::length(m[2]);

	/* and the sign of the scaling */
	if (glm::determinant(t) < 0)
	{
		scale = -scale;
	}

	/* and remove all scaling from the matrix */
	if (scale.x) m[0] /= scale.x;
	if (scale.y) m[1] /= scale.y;
	if (scale.z) m[2] /= scale.z;

	q = glm::quat{ m };
}

void glm_base::MatrixRotateZYX(glm::mat4x4& _result, float _ax, float _ay, float _az)
{
	_result = MatrixRotateZYX(_ax, _ay, _az);
}


