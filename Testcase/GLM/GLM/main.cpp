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


