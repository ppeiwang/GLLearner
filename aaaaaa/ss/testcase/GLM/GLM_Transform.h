#pragma once
#include <glm/glm.hpp>
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/quaternion.hpp"
#include <iostream>
#include "TextTable.h"
#include <iomanip>

namespace glm_base
{
	class Transform
	{
	private:
		typedef size_t length_type;
		typedef float value_type;

	public:
		Transform() = default;

		~Transform() = default;

		explicit Transform(const glm::mat4x4& mat)
			: data(mat)
		{

		}

		value_type& operator [](length_type i)
		{
			return *(&data[0][0] + i);
		}

		const value_type& operator [](length_type i) const
		{
			return *(&data[0][0] + i);
		}

		Transform& operator = (const glm::mat4x4& mat)
		{
			data = mat;
			return *this;
		}

		bool operator == (const Transform& other)
		{
			return data == other.data;
		}

		bool operator != (const Transform& other)
		{
			return data != other.data;
		}

		operator glm::mat4x4& ()
		{
			return data;
		}

		operator const glm::mat4x4& () const
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

		glm::mat4x4 data;

	private:
		std::vector<size_t> GetColunm(size_t n) const
		{
			std::vector<size_t> c;

			for (size_t i = 0; i < 4; i++)
			{
				c.push_back(n * 4 + i);
			}
			return c;
		}

		std::vector<size_t> GetRow(size_t n) const
		{
			std::vector<size_t> r;

			for (size_t i = 0; i < 4; i++)
			{
				r.push_back(i * 4 + n);
			}
			return r;
		}
	};

	void MatrixScale(glm::mat4x4& _result, float _sx, float _sy, float _sz);

	void MatrixTranslate(glm::mat4x4& _result, float _tx, float _ty, float _tz);

	void MatrixTranspose(glm::mat4x4& _result, const glm::mat4x4& _a);

	void MatrixRotateXYZ(glm::mat4x4& _result, float _ax, float _ay, float _az);

	void MatrixRotateZYX(glm::mat4x4& _result, float _ax, float _ay, float _az);

	glm::mat4x4 MatrixTranslate(float _tx, float _ty, float _tz);

	glm::mat4x4 MatrixScale(float _sx, float _sy, float _sz);

	glm::mat4x4 MatrixIdentity();

	glm::mat4x4 MatrixLookAt(const glm::vec3& _eye, const glm::vec3& _at, const glm::vec3& _up);

	glm::mat4x4 MatrixRotateXYZ(float _ax, float _ay, float _az);

	glm::mat4x4 MatrixRotateZYX(float _ax, float _ay, float _az);

	glm::mat4x4 MatrixRotateX(float _ax);

	glm::mat4x4 MatrixRotateY(float _ay);

	glm::mat4x4 MatrixRotateZ(float _az);

	void MatrixDecompose(const glm::mat4x4& _mat, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

	void MatrixDecompose(const glm::mat4x4& _mat, glm::vec3& translation, glm::quat& q, glm::vec3& scale);
}
