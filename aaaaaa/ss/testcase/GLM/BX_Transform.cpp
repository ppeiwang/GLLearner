#include "BX_Transform.h"
#include <assert.h>

namespace bx
{

	void bx::vec4MulMtx(float* _result, const float* _vec, const float* _mat)
	{
		_result[0] = _vec[0] * _mat[0] + _vec[1] * _mat[4] + _vec[2] * _mat[8] + _vec[3] * _mat[12];
		_result[1] = _vec[0] * _mat[1] + _vec[1] * _mat[5] + _vec[2] * _mat[9] + _vec[3] * _mat[13];
		_result[2] = _vec[0] * _mat[2] + _vec[1] * _mat[6] + _vec[2] * _mat[10] + _vec[3] * _mat[14];
		_result[3] = _vec[0] * _mat[3] + _vec[1] * _mat[7] + _vec[2] * _mat[11] + _vec[3] * _mat[15];
	}

	void bx::mtxMul(float* _result, const float* _a, const float* _b)
	{
		vec4MulMtx(&_result[0], &_a[0], _b);
		vec4MulMtx(&_result[4], &_a[4], _b);
		vec4MulMtx(&_result[8], &_a[8], _b);
		vec4MulMtx(&_result[12], &_a[12], _b);
	}

	void mtxIdentity(float* _result)
	{
		memset(_result, 0, sizeof(float) * 16);
		_result[0] = _result[5] = _result[10] = _result[15] = 1.0f;
	}

	void mtxTranslate(float* _result, float _tx, float _ty, float _tz)
	{
		mtxIdentity(_result);
		_result[12] = _tx;
		_result[13] = _ty;
		_result[14] = _tz;
	}

	void mtxLookAt(float* _result, const Vec3& _eye, const Vec3& _at, const Vec3& _up)
	{
		const Vec3 view = normalize(sub(_eye, _at));
		const Vec3 uxv = cross(_up, view);
		const Vec3 right = normalize(uxv);
		const Vec3 up = cross(view, right);

		memset(_result, 0, sizeof(float) * 16);
		_result[0] = right.x;
		_result[1] = up.x;
		_result[2] = view.x;

		_result[4] = right.y;
		_result[5] = up.y;
		_result[6] = view.y;

		_result[8] = right.z;
		_result[9] = up.z;
		_result[10] = view.z;

		_result[12] = -dot(right, _eye);
		_result[13] = -dot(up, _eye);
		_result[14] = -dot(view, _eye);
		_result[15] = 1.0f;
	}

	void mtxScale(float* _result, float _sx, float _sy, float _sz)
	{
		memset(_result, 0, sizeof(float) * 16);
		_result[0] = _sx;
		_result[5] = _sy;
		_result[10] = _sz;
		_result[15] = 1.0f;
	}

	void mtxTranspose(float* _result, const float* _a)
	{
		_result[0] = _a[0];
		_result[4] = _a[1];
		_result[8] = _a[2];
		_result[12] = _a[3];
		_result[1] = _a[4];
		_result[5] = _a[5];
		_result[9] = _a[6];
		_result[13] = _a[7];
		_result[2] = _a[8];
		_result[6] = _a[9];
		_result[10] = _a[10];
		_result[14] = _a[11];
		_result[3] = _a[12];
		_result[7] = _a[13];
		_result[11] = _a[14];
		_result[15] = _a[15];
	}

	void mtxRotateXYZ(float* _result, float _ax, float _ay, float _az)
	{
		const float sx = sin(_ax);
		const float cx = cos(_ax);
		const float sy = sin(_ay);
		const float cy = cos(_ay);
		const float sz = sin(_az);
		const float cz = cos(_az);

		memset(_result, 0, sizeof(float) * 16);

		_result[0] = cy * cz;
		_result[1] = -cy * sz;
		_result[2] = sy;
		_result[4] = cz * sx * sy + cx * sz;
		_result[5] = cx * cz - sx * sy * sz;
		_result[6] = -cy * sx;
		_result[8] = -cx * cz * sy + sx * sz;
		_result[9] = cz * sx + cx * sy * sz;
		_result[10] = cx * cy;
		_result[15] = 1.0f;
	}

	Vec3::Vec3() :
		x{}, y{}, z{}
	{

	}

	constexpr Vec3::Vec3(float _v)
		: x(_v)
		, y(_v)
		, z(_v)
	{
	}

	constexpr Vec3::Vec3(float _x, float _y, float _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{
	}

	float& Vec3::operator[](size_t i)
	{
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			break;
		}
		assert(0);
		return x;
	}

	const float& Vec3::operator[](size_t i) const
	{
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			break;
		}
		assert(0);
		return x;
	}

}
