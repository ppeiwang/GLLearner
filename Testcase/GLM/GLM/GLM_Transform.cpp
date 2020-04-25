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
	glm::vec3 vCols[3] = {
		glm::vec3{t[0][0], t[0][1], t[0][2]},
		glm::vec3{t[1][0], t[1][1], t[1][2]},
		glm::vec3{t[2][0], t[2][1], t[2][2]}
	};

	/* extract the scaling factors */
	scale.x = glm::length(vCols[0]);
	scale.y = glm::length(vCols[1]);
	scale.z = glm::length(vCols[2]);

	/* and the sign of the scaling */
	if (glm::determinant(t) < 0)
	{
		scale = -scale;
	}

	/* and remove all scaling from the matrix */
	if (scale.x) vCols[0] /= scale.x;
	if (scale.y) vCols[1] /= scale.y;
	if (scale.z) vCols[2] /= scale.z;

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

	rotation.y = std::asin(-vCols[0].z);// D. Angle around Y-axis.

	const auto C = std::cos(rotation.y);

	if (std::fabs(C) > glm::epsilon<float>())
	{
		// Finding angle around X-axis.
		auto tan_x = vCols[2].z / C;	// A
		auto tan_y = vCols[1].z / C;	// B

		rotation.x = std::atan2(tan_y, tan_x);
		// Finding angle around Z-axis.
		tan_x = vCols[0].x / C;	// E
		tan_y = vCols[0].y / C;	// F
		rotation.z = std::atan2(tan_y, tan_x);
	}
	else
	{
		// Y-axis is fixed.
		rotation.x = 0;// Set angle around oX to 0. => A == 1, B == 0, C == 0, D == 1.

		// And finding angle around Z-axis.
		const auto tan_x = vCols[1].y;		// BDF+AE => E
		const auto tan_y = -vCols[1].x;		// BDE-AF => F

		rotation.z = std::atan2(tan_y, tan_x);
	}

#if 0
	Transform t{ _mat };

	/* extract translation */
	p.x = t[12];
	p.y = t[13];
	p.z = t[14];

	/* extract the columns of the matrix. */
	glm::vec3 vCols[3] = {
		glm::vec3{t[0], t[1], t[2]},
		glm::vec3{t[4], t[5], t[6]},
		glm::vec3{t[8], t[9], t[10]}
	};

	/* extract the scaling factors */
	s.x = glm::length(vCols[0]);
	s.y = glm::length(vCols[1]);
	s.z = glm::length(vCols[2]);

	/* and the sign of the scaling */
	if (glm::determinant(t.data) < 0)
	{
		s = -s;
	}

	/* and remove all scaling from the matrix */
	if (s.x) vCols[0] /= s.x;
	if (s.y) vCols[1] /= s.y;
	if (s.z) vCols[2] /= s.z;

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

	r.y = std::asin(-vCols[0].z);// D. Angle around oY.

	const auto C = std::cos(r.y);

	if (std::fabs(C) > glm::epsilon<float>())
	{
		// Finding angle around oX.
		auto tan_x = vCols[2].z / C;	// A
		auto tan_y = vCols[1].z / C;	// B

		r.x = std::atan2(tan_y, tan_x);
		// Finding angle around oZ.
		tan_x = vCols[0].x / C;	// E
		tan_y = vCols[0].y / C;	// F
		r.z = std::atan2(tan_y, tan_x);
	}
	else
	{
		// oY is fixed.
		r.x = 0;// Set angle around oX to 0. => A == 1, B == 0, C == 0, D == 1.

		// And finding angle around oZ.
		const auto tan_x = vCols[1].y;		// BDF+AE => E
		const auto tan_y = -vCols[1].x;		// BDE-AF => F

		r.z = std::atan2(tan_y, tan_x);
	}
#endif

#if 0 
	p.x = t[12];
	p.y = t[13];
	p.z = t[14];
	s.x = std::sqrt(t[0] * t[0] + t[1] * t[1] + t[2] * t[2] + t[3] * t[3]);
	s.y = std::sqrt(t[4] * t[4] + t[5] * t[5] + t[6] * t[6] + t[7] * t[7]);
	s.z = std::sqrt(t[8] * t[8] + t[9] * t[9] + t[10] * t[10] + t[11] * t[11]);

	float m00 = t[0] / s.x;
	float m10 = t[1] / s.x;
	float sy = (float)sqrt(m00 * m00 + m10 * m10);
	if (sy < 1e-6)
	{
		r.x = (float)atan2(-t[9] / s.z, t[5] / s.y);
		r.y = (float)atan2(-t[2] / s.x, sy);
		r.z = 0;
	}
	else
	{
		r.x = (float)atan2(t[6] / s.y, (t[10] / s.z));
		r.y = (float)atan2(-t[2] / s.x, sy);
		r.z = (float)atan2(t[1], t[0]);
	}
#endif
}

void glm_base::MatrixRotateZYX(glm::mat4x4& _result, float _ax, float _ay, float _az)
{
	_result = MatrixRotateZYX(_ax, _ay, _az);
}


