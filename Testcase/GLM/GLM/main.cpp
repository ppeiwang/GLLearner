#include <iostream>
#include <string>
#include <functional>
#include "GLM_Transform.h"
#include "BX_Transform.h"
#include "TestCase.h"
#include "StringMatrix.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <list>
#include <vector>
#include <algorithm>
#include <memory>
#include <random>


int main()
{
	std::list<float> list {1.0f, 1.1f, 1.2f, 1.3f};

	auto itr = std::remove(list.begin(), list.end(), 1.1f);



	{
		auto m = glm::eulerAngleXYZ(0.f, glm::pi<float>(),0.f);

		auto q = glm::quat{ m };

		int a = 0;
	}


	{
		auto Rand = [](int min, int max, size_t len, size_t base) -> auto
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(min, max);
			std::vector<float> vec;
			for (size_t i = 0; i < len; i++)
				vec.push_back((float)dis(gen) / (float)base);

			return vec;
		};


		auto m1 = glm::scale(glm::identity<glm::mat4>(), glm::vec3{ 1.f, 1.f, -1.f });

		auto m_inv = glm::inverse(m1);

		while (true)
		{
			auto v = Rand(0, 500, 4, 500);
			glm::quat q { v[0], v[1], v[2], v[3] };

			q = glm::normalize(q);

			glm::mat4 m{ q };

			glm::quat q0{ m };
			glm::quat q1{ m1 * m *m1};


			int a = 0;

		}


	
		int i = 0;
	}


	int i = 0;

#if 1	
	{
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
	}

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


