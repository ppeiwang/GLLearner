#pragma once
#include <iostream>
#include "TextTable.h"
#include <iomanip>

namespace bx
{
	struct Transform
	{
		const static int TRANSFORM_DATA_SIZE = 16;

		float data[TRANSFORM_DATA_SIZE] = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };

		bool operator==(const Transform& b) const
		{
			for (int i = 0; i < TRANSFORM_DATA_SIZE; i++)
				if (data[i] != b.data[i])
					return false;
			return true;
		}
		bool operator!=(const Transform& b) const
		{
			for (int i = 0; i < TRANSFORM_DATA_SIZE; i++)
				if (data[i] != b.data[i])
					return true;
			return false;
		}

		float& operator [](size_t i)
		{
			return data[i];
		}

		const float& operator [](size_t i) const
		{
			return data[i];
		}

		operator float* ()
		{
			return data;
		}

		operator const float* () const
		{
			return data;
		}

		void Print() const
		{
			TextTable t('-', '|', '+');

			for (size_t i = 0; i < 4; i++)
			{
				auto rVec = GetRow(i);
				for (auto r : rVec)
				{
					t.add(std::to_string(operator[](r)));
				}
				t.endOfRow();
			}

			t.setAlignment(2, TextTable::Alignment::RIGHT);

			std::cout << t << std::endl;
		}

		void Print(int) const
		{
			Print();
		}

	private:
		std::vector<size_t> GetColunm(size_t n) const
		{
			std::vector<size_t> c;
			for (size_t i = 0; i < 4; i++)
			{
				c.push_back(i * 4 + n);
			}
			return c;
		}

		std::vector<size_t> GetRow(size_t n) const
		{
			std::vector<size_t> r;
			for (size_t i = 0; i < 4; i++)
			{
				r.push_back(n * 4 + i);
			}

			return r;
		}
	};

	struct Vec3
	{
		///
		Vec3();

		explicit constexpr Vec3(float _v);

		constexpr Vec3(float _x, float _y, float _z);

		float x, y, z;

		float& operator[](size_t i);

		const float& operator[](size_t i) const;
	};

	inline float dot(const Vec3 _a, const Vec3 _b)
	{
		return _a.x * _b.x + _a.y * _b.y + _a.z * _b.z;
	}

	inline Vec3 cross(const Vec3 _a, const Vec3 _b)
	{
		return
		{
			_a.y * _b.z - _a.z * _b.y,
			_a.z * _b.x - _a.x * _b.z,
			_a.x * _b.y - _a.y * _b.x,
		};
	}

	inline Vec3 mul(const Vec3 _a, float _b)
	{
		return
		{
			_a.x * _b,
			_a.y * _b,
			_a.z * _b,
		};
	}

	inline Vec3 sub(const Vec3 _a, const Vec3 _b)
	{
		return
		{
			_a.x - _b.x,
			_a.y - _b.y,
			_a.z - _b.z,
		};
	}

	inline float length(const Vec3 _a)
	{
		return sqrt(dot(_a, _a));
	}

	inline Vec3 normalize(const Vec3 _a)
	{
		const float invLen = 1.0f / length(_a);
		const Vec3 result = mul(_a, invLen);
		return result;
	}

	void vec4MulMtx(float* _result, const float* _vec, const float* _mat);

	void mtxMul(float* _result, const float* _a, const float* _b);

	void mtxRotateXYZ(float* _result, float _ax, float _ay, float _az);

	void mtxIdentity(float* _result);

	void mtxTranslate(float* _result, float _tx, float _ty, float _tz);

	void mtxLookAt(float* _result, const Vec3& _eye, const Vec3& _at, const Vec3& _up);

	void mtxScale(float* _result, float _sx, float _sy, float _sz);

	void mtxTranspose(float* _result, const float* _a);
}

