#include <iostream>
#include <string>
#include "GLM_Transform.h"
#include "BX_Transform.h"
#include "TestCase.h"
#include "StringMatrix.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

namespace ASSIM
{
	class aiMatrix4x4t
	{
	public:
		typedef float TReal;

		aiMatrix4x4t(TReal _a1, TReal _a2, TReal _a3, TReal _a4,
			TReal _b1, TReal _b2, TReal _b3, TReal _b4,
			TReal _c1, TReal _c2, TReal _c3, TReal _c4,
			TReal _d1, TReal _d2, TReal _d3, TReal _d4) :
			a1(_a1), a2(_a2), a3(_a3), a4(_a4),
			b1(_b1), b2(_b2), b3(_b3), b4(_b4),
			c1(_c1), c2(_c2), c3(_c3), c4(_c4),
			d1(_d1), d2(_d2), d3(_d3), d4(_d4) {
			// empty
		}

		TReal* operator[](unsigned int p_iIndex) {
			if (p_iIndex > 3) {
				return nullptr;
			}
			switch (p_iIndex) {
			case 0:
				return &a1;
			case 1:
				return &b1;
			case 2:
				return &c1;
			case 3:
				return &d1;
			default:
				break;
			}
			return &a1;
		}

		TReal a1, a2, a3, a4;
		TReal b1, b2, b3, b4;
		TReal c1, c2, c3, c4;
		TReal d1, d2, d3, d4;
	};
}

class OperatorTest
{
public:
	OperatorTest() = default;

	OperatorTest(int _x, int _y, int _z) :
		x(_x), y(_y), z(_z)
	{

	}

	~OperatorTest() = default;

	OperatorTest operator + (const OperatorTest& o) const
	{
		return { x + o.x, y + o.y, z + o.z };
	}

	void Print()
	{
		TextTable t('-', '|', '+');
		t.add(std::to_string(x));
		t.add(std::to_string(y));
		t.add(std::to_string(z));
		t.endOfRow();
		t.setAlignment(2, TextTable::Alignment::RIGHT);
		std::cout << t << std::endl;
	}

	int x, y, z;
};

int main()
{
	float x = 368536.493263f - 368394.0f;
	float y = 3459015.067785f - 3459110.0f;
	float z = 17.0110265f;
	float half_pi = -1.570796f;
	glm::vec4 p{x, y, z, 0.0f};
	glm::vec3 r_adjust = { half_pi, 0.0f, 0.0f };
	auto m_hwspace_2_gwcpS = glm::eulerAngleZYX(r_adjust.z, r_adjust.y, r_adjust.x);
	p = m_hwspace_2_gwcpS*p;
	int i = 0;
	// Scene 1
	{
		float m0 = 0.9986;
		float m1 = -0.0174f;
		float m2 = -0.0482f;
		float m3 = 0.0f;
		float m4 = 0.0489f;
		float m5 = 0.4636f;
		float m6 = 0.9977f;
		float m7 = 0.0f;
		float m8 = -0.0151f;
		float m9 = -0.9987f;
		float m10 = 0.0471f;
		float m11 = 0.0f;
		float m12 = 0.0f;
		float m13 = 0.0f;
		float m14 = 0.0f;
		float m15 = 1.0f;

		glm::mat4 matrix{ 1.0f };

		auto Set = [&](float f, size_t index) {
			(&matrix[0][0])[index] = f;
		};

		Set(m0, 0);
		Set(m1, 1);
		Set(m2, 2);
		Set(m3, 3);
		Set(m4, 4);
		Set(m5, 5);
		Set(m6, 6);
		Set(m7, 7);
		Set(m8, 8);
		Set(m9, 9);
		Set(m10, 10);
		Set(m11, 11);
		Set(m12, 12);
		Set(m13, 13);
		Set(m14, 14);
		Set(m15, 15);

		float x, y, z;
		glm::extractEulerAngleZYX(matrix, z, y, x);
		std::cout << "scene1: " << "x: " << x << ", y: " << y << ", z: " << z << std::endl;

		const auto matrix_transpose = glm::transpose(matrix);

		glm::extractEulerAngleZYX(matrix_transpose, z, y, x);
		std::cout << "scene1 after transpose : " << "x: " << x << ", y: " << y << ", z: " << z << std::endl;

		const auto north = glm::vec4{ 0.f, 1.0f, 0.f, 1.0f};
		auto north_inverse_transform = glm::inverse(matrix) * north;
		std::cout << "scene1: north after inverse transform. x: " << north_inverse_transform.x << ", y: " << north_inverse_transform.y << ", z: " << north_inverse_transform.z << std::endl;
		north_inverse_transform = glm::inverse(matrix_transpose) * north;
		std::cout << "scene1: north after transpose inverse transform. x: " << north_inverse_transform.x << ", y: " << north_inverse_transform.y <<", z: " << north_inverse_transform.z << std::endl;

		std::cout << std::endl;
	}


	// Scene 2
	{
		float m0 = 0.00043f;
		float m1 = 0.994615f;
		float m2 = -0.103635f;
		float m3 = 0.0f;
		float m4 = -0.16867f;
		float m5 = -0.102077f;
		float m6 = -0.98037f;
		float m7 = 0.0f;
		float m8 = -0.98567f;
		float m9 = 0.01790f;
		float m10 = 0.16772f;
		float m11 = 0.0f;
		float m12 = 0.0f;
		float m13 = 0.0f;
		float m14 = 0.0f;
		float m15 = 1.0f;

		glm::mat4 matrix{ 1.0f };

		auto Set = [&](float f, size_t index) {
			(&matrix[0][0])[index] = f;
		};

		Set(m0, 0);
		Set(m1, 1);
		Set(m2, 2);
		Set(m3, 3);
		Set(m4, 4);
		Set(m5, 5);
		Set(m6, 6);
		Set(m7, 7);
		Set(m8, 8);
		Set(m9, 9);
		Set(m10, 10);
		Set(m11, 11);
		Set(m12, 12);
		Set(m13, 13);
		Set(m14, 14);
		Set(m15, 15);

		float x, y, z;
		glm::extractEulerAngleZYX(matrix, z, y, x);
		std::cout << "scene2: "  << "x: " << x << ", y: " << y << ", z: " << z << std::endl;

		const auto matrix_transpose = glm::transpose(matrix);

		glm::extractEulerAngleZYX(matrix_transpose, z, y, x);
		std::cout << "scene2 after transpose : " << "x: " << x << ", y: " << y << ", z: " << z << std::endl;

		const auto south = glm::vec4{ 0.f, -1.0f, 0.f, 1.0f };
		auto south_inverse_transform = glm::inverse(matrix) * south;
		std::cout << "scene2: south after inverse transform. x: " << south_inverse_transform.x << ", y: " << south_inverse_transform.y << ", z: " << south_inverse_transform.z << std::endl;
		south_inverse_transform = glm::inverse(matrix_transpose) * south;
		std::cout << "scene2: south after transpose inverse transform. x: " << south_inverse_transform.x << ", y: " << south_inverse_transform.y << ", z: " << south_inverse_transform.z << std::endl;

		std::cout << std::endl;
	}

	// Scene 3
	{
		float m0 = -0.10501f;
		float m1 = -0.378678f;
		float m2 = -0.91955f;
		float m3 = 0.0f;
		float m4 = -0.03149f;
		float m5 = -0.922933f;
		float m6 = 0.38366f;
		float m7 = 0.0f;
		float m8 = -0.9939f;
		float m9 = 0.06925f;
		float m10 = 0.08499f;
		float m11 = 0.0f;
		float m12 = 0.0f;
		float m13 = 0.0f;
		float m14 = 0.0f;
		float m15 = 1.0f;

		glm::mat4 matrix{ 1.0f };

		auto Set = [&](float f, size_t index) {
			(&matrix[0][0])[index] = f;
		};

		Set(m0, 0);
		Set(m1, 1);
		Set(m2, 2);
		Set(m3, 3);
		Set(m4, 4);
		Set(m5, 5);
		Set(m6, 6);
		Set(m7, 7);
		Set(m8, 8);
		Set(m9, 9);
		Set(m10, 10);
		Set(m11, 11);
		Set(m12, 12);
		Set(m13, 13);
		Set(m14, 14);
		Set(m15, 15);

		float x, y, z;
		glm::extractEulerAngleZYX(matrix, z, y, x);
		std::cout << "scene3 : " << "x: " << x << ", y: " << y << ", z: " << z << std::endl;

		const auto matrix_transpose = glm::transpose(matrix);

		glm::extractEulerAngleZYX(matrix_transpose, z, y, x);
		std::cout << "scene3 after transpose : " << "x: " << x << ", y: " << y << ", z: " << z << std::endl;

		const auto west = glm::vec4{ -1.f, 0.0f, 0.f, 1.0f };
		auto west_inverse_transform = glm::inverse(matrix) * west;
		west_inverse_transform = glm::normalize(west_inverse_transform);
		std::cout << "scene3: west after inverse transform. x: " << west_inverse_transform.x << ", y: " << west_inverse_transform.y << ", z: " << west_inverse_transform.z << std::endl;
		west_inverse_transform = glm::inverse(matrix_transpose) * west;
		west_inverse_transform = glm::normalize(west_inverse_transform);
		std::cout << "scene3: west after transpose inverse transform. x: " << west_inverse_transform.x << ", y: " << west_inverse_transform.y << ", z: " << west_inverse_transform.z << std::endl;
	}

	return 0;

#if 1	
	Test_Matrix_Mul();
	Test_Matrix_Identity();
	Test_Matrix_Lookat();
	Test_Matrix_Scale();
	Test_Matrix_Translate();
	Test_Matrix_Transpose();
	Test_Matrix_RotateXYZ();
	Test_Matrix_InverseTranspose();
	Test_Matrix_Make();
	Test_Matrix_Decompose();

	Test_Matrix_GLM_EulerMatrix();
	Test_Matrix_Rotation_Decompose();
	Test_Matrix_Rotate_Axis();
	Test_Quaternion_FromMatrix();
	Test_Quaternion_ToMatrix();

	Test_Quaternion2Euler();
	Test_Euler2Quaternion();

	Test_GimbalLock();


	std::cout << "Everything is ok!" << std::endl;

	int a = 0;


#else
	StringMatrix<4> SMat_x = StringMatrix<4>::RotateX("x");
	SMat_x.Print();

	StringMatrix<4> SMat_y = StringMatrix<4>::RotateY("y");
	SMat_y.Print();

	StringMatrix<4> SMat_z = StringMatrix<4>::RotateZ("z");
	SMat_z.Print();

	std::cout << "X*Y" << std::endl;
	auto m_xy = SMat_x * SMat_y;
	m_xy.Print();

	std::cout << "X*Y*Z" << std::endl;

	auto m_xyz = SMat_x * SMat_y * SMat_z;
	m_xyz.Print();

	std::cout << "Z*Y*X" << std::endl;
	auto m_zyx = SMat_z * SMat_y * SMat_x;
	m_zyx.Print();
		
	const auto SMat_x_t = SMat_x.Transpose();
	const auto SMat_y_t = SMat_y.Transpose();
	const auto SMat_z_t = SMat_z.Transpose();

	std::cout << "Transpose(X*Y*Z)" << std::endl;
	auto m_t_zyx = SMat_z_t * SMat_y_t * SMat_x_t;
	m_t_zyx.Print();

	std::cout << "XYX is" << std::endl;

	const auto SMat_x_3 = StringMatrix<4>::RotateX("3");
	auto m_xyx = SMat_x * SMat_y * SMat_x_3;

	m_xyx.Print();


#endif
	
	return 0;
}


