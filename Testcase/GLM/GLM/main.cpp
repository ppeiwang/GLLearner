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
	// quat which can rotate v1 to v2
	// v1 and v2 are not 
	auto GetQuat = [](const glm::vec3& v1, const glm::vec3& v2) {
		auto v1_n = glm::normalize(v1);
		auto v2_n = glm::normalize(v2);
		const auto dot = glm::dot(v1_n, v2_n);
		if (glm::epsilonEqual(dot, 1.0f, glm::epsilon<float>()))
		{
			return glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f };
		}
		else if (glm::epsilonEqual(dot, -1.0f, glm::epsilon<float>()))
		{
			if (glm::epsilonEqual(v2_n.x, 0.0f, glm::epsilon<float>()))
			{
				v2_n.x = glm::epsilon<float>();
				v2_n = glm::normalize(v2_n);
			}
			else
			{
				v2_n.x = 0.0f;
				v2_n.y = glm::epsilon<float>();
				v2_n = glm::normalize(v2_n);
			}
		}

		const auto axis = glm::normalize(glm::cross(v1_n, v2_n));
		const auto theta = glm::acos(glm::dot(v1_n, v2_n));
		return glm::quat{ cos(theta * 0.5f), axis * sin(theta * 0.5f) };
	};

	{
		const auto m0 = glm::eulerAngleXYZ(glm::half_pi<float>(), 0.0f, 0.0f);
		const auto m1 = glm::eulerAngleZYX(0.0f, glm::half_pi<float>(), 0.0f);
		const auto m2 = m0 * m1;

		const auto q_0 = glm::quat{ m0 };
		const auto q_1 = glm::quat{ m1 };
		const auto q_2 = glm::quat{ m2 };

		const auto v0 = glm::vec4{0.f, 0.f, 1.f, 0.0f};

		const auto v1 = glm::vec4{0.f, 0.f, -1.f, 0.0f};

		const auto q_v0_2_v1 = GetQuat(glm::vec3{ v0 }, glm::vec3{v1});
		const auto m_v0_2_v1 = glm::mat3{ q_v0_2_v1 };

		const auto v2 = m_v0_2_v1 * v0;


		v2;
		int i = 0;
	}







	const auto v1 = glm::vec3{ 1.0f, 0.0f, 0.0f };
	const auto v2 = glm::vec3{ 0.0f, 1.0f, 0.0f };
	
	auto r_qua = GetQuat(v1, v2);

	const auto m_r = glm::mat3{ r_qua};
	const auto v3 = m_r * v1;


	auto m0 = glm::eulerAngleXYZ(glm::pi<float>(), 0.0f, 0.0f);
	auto m1 = glm::eulerAngleXYZ(glm::half_pi<float>(), 0.0f, 0.0f);
	auto m2 = glm::eulerAngleXYZ(0.0f, glm::half_pi<float>(), 0.0f);

	auto q0 =  glm::quat(m0);
	auto q1 = glm::quat(m1);
	auto q2 = glm::quat(m2);

	auto q_east = glm::quat{ 0.67f, 0.71f, -0.16f, 0.15f };
	auto q_west = glm::quat{ 0.37f, 0.36f, 0.60f, -0.61f };

	//auto q_east_n = glm::normalize(q_east);
	//auto q_west_n = glm::normalize(q_west);

	q_east = glm::normalize(q_east);
	q_west = glm::normalize(q_west);

	auto v = glm::dot(q_west,q_east);

	auto m_east = glm::mat4{q_east};
	auto m_west = glm::mat4{q_west};

	auto v_east = m_east * glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f };
	auto v_west = m_west * glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f };

	auto v_east1 = m_east * glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f };
	auto v_west1 = m_west * glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f };

	auto compare = glm::epsilonEqual(v_east, v_west, 0.001f);
	auto compar1 = glm::epsilonEqual(v_east1, v_west1, 0.001f);

	auto v_east_0 = glm::conjugate(q_east) * glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f } *q_east;
	auto v_west_0 = glm::conjugate(q_west) * glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f } *q_west;

	auto v_west_10 = m0 * glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f };
	auto v_west_11 = m1 * glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f };
	auto v_west_1 = glm::conjugate(q0) * glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f } *q0;
	auto v_west_2 = glm::conjugate(q1) * glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f } *q0;

	Test_RigionRotate();

	const auto c_theta = glm::cos(glm::pi<float>()/3);
	const auto s_theta = glm::sin(glm::pi<float>()/3);

	const auto vec_u = glm::vec3(c_theta, s_theta, 0);
	const auto vec_up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat3 m = glm::eulerAngleZYX(glm::pi<float>()/3, 0.0f, 0.0f);

//	const auto lm = glm::lookAtRH(glm::vec3{ 0 }, -m[2], m[1]);
	auto lookAtMatrix = [](glm::vec3& view, glm::vec3& up) {
		glm::mat3 matrix{ 1 };
		glm::vec3 z = glm::normalize(view);
		glm::vec3 x = glm::normalize(glm::cross(up, z));
		glm::vec3 y = glm::cross(z, x);
		matrix[0] = x;
		matrix[1] = y;
		matrix[2] = z;
		return matrix;
	};

	const auto lm = lookAtMatrix(m[2], m[1]);

	const auto world_2_local = glm::transpose(m);
	const auto local_2_world = glm::mat3{ lm };

	const auto local = world_2_local * vec_u; // should be x_axis
	const auto world = local_2_world * glm::vec3{ 1.f, 0.0f, 0 };

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

		const auto vec_t = matrix_transpose * glm::vec4{0.0f, 0.0f, 1.0f, 1.0f};

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


