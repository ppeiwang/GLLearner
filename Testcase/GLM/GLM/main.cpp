#include <iostream>
#include <string>
#include "GLM_Transform.h"
#include "BX_Transform.h"
#include "TestCase.h"
#include "StringMatrix.h"
#include "glm/gtc/quaternion.hpp"

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
#if 0
	Test_Matrix_Rotation_Decompose();
	Test_Matrix_Rotate_Axis();
	Test_Quaternion_FromMatrix();
	Test_Quaternion_ToMatrix();
	Test_Quaternion_ToEuler();

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

	std::cout << "Everything is ok!" << std::endl;

	//ASSIM::aiMatrix4x4t aiM{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	//
	//glm_base::Transform t;
	//std::copy(aiM[0], aiM[3] + 4, &t[0]);

	//t.Print();

	const glm::vec4 v { 1.0f, 0.f, 0.f, 0.0f};

	const auto half_pi = glm::pi<float>() / 2.0f;

	const auto mx = glm::rotate(glm::mat4{ 1 }, 0.0f, glm::vec3{ 1.0f,  0.0f, 0.0f });
	const auto my = glm::rotate(glm::mat4{ 1 }, glm::pi<float>() / 2.0f, glm::vec3{ 0.0f,  1.0f, 0.0f });
	const auto mz = glm::rotate(glm::mat4{ 1 }, glm::pi<float>() / 2.0f, glm::vec3{ 0.0f,  0.0f, 1.0f });

	const auto mzyx = glm_base::MatrixRotateZYX(0.f, half_pi, half_pi);

	const auto m = mz * my * mx;

	const auto result = m*v;
	const auto resutl1 = mzyx * v;

	int a = 0;


#else
	StringMatrix<4> SMat_x;
	SMat_x.Construct(std::vector<string>({"1","0","0","0", "0","c1","-s1","0", "0","s1","c1","0", "0","0","0","1"}));
	SMat_x.Print();

	StringMatrix<4> SMat_y;
	SMat_y.Construct(std::vector<string>({"c2","0","s2","0", "0","1","0","0", "-s2","0","c2","0", "0","0","0","1"}));
	SMat_y.Print();

	StringMatrix<4> SMat_z;
	SMat_z.Construct(std::vector<string>({"c3","-s3","0","0", "s3","c3","0","0", "0","0","1","0", "0","0","0","1"}));
	SMat_z.Print();

	std::cout << "colunm-major matrix rotation(glm) x*y*z: " << std::endl;

	auto m_xyz = SMat_x * SMat_y * SMat_z;
	m_xyz.Print();


	std::cout << "colunm-major matrix rotation(glm) z*y*x: " << std::endl;

	auto m0 = SMat_z * SMat_y * SMat_x;

	m0.Print();
		
	auto SMat_x_t = SMat_x.Transpose();
	SMat_x_t.Print();

	auto SMat_y_t = SMat_y.Transpose();
	SMat_y_t.Print();

	auto SMat_z_t = SMat_z.Transpose();
	SMat_z_t.Print();

	std::cout << "row-major matrix rotation (x*y*z)T: " << std::endl;
	auto m1 = SMat_z_t * SMat_y_t * SMat_x_t;
	//auto m1 = SMat_x_t * SMat_y_t * SMat_z_t;
	m1.Print();

	std::cout << "the transpose of result matrix" << std::endl;
	auto m3 = SMat_x_t * SMat_y_t * SMat_z_t;
	auto m2 = m3.Transpose();
	m2.Print();



#endif
	
	return 0;
}


