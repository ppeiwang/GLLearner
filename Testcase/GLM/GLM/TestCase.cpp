#include "TestCase.h"
#include <random>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtx/euler_angles.hpp"


const size_t G_INTENSITY = 1000;

std::vector<float> RangeGenerator(float min, float max, const size_t len /*= 16*/)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	std::vector<float> vec;
	for (size_t i = 0; i < len; i++)
		vec.push_back(dis(gen));

	return vec;
}

std::vector<float> SerialGenerator(const size_t len /*= 16*/)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(-100, 100);
	std::vector<float> vec;
	for (size_t i = 0; i < len; i++)
		vec.push_back(dis(gen));

	return vec;
}

std::vector<float> PositiveSerialGenerator(const size_t len /*= 16*/)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 100);
	std::vector<float> vec;
	for (size_t i = 0; i < len; i++)
		vec.push_back(dis(gen));

	return vec;
}

template<>
bool CompareRadian(const float& radian0, const float& radian1)
{
	const auto c0 = glm::cos(radian0);
	const auto c1 = glm::cos(radian1);
	const auto s0 = glm::sin(radian0);
	const auto s1 = glm::sin(radian1);

	return  std::abs(glm::cos(radian0) - glm::cos(radian1)) < glm::epsilon<float>() &&
		std::abs(glm::sin(radian0) - glm::sin(radian1)) < glm::epsilon<float>();
}


void Test_Matrix_Mul()
{
	bool bFlag = true;
	for (size_t i = 0; i < G_INTENSITY; i++)
	{
		const auto v0 = SerialGenerator();
		const auto v1 = SerialGenerator();
		const auto v2 = SerialGenerator();

		const glm_base::Transform glm_t0 = MakeMatrix<glm_base::Transform>(v0);
		const glm_base::Transform glm_t1 = MakeMatrix<glm_base::Transform>(v1);
		const glm_base::Transform glm_t2 = MakeMatrix<glm_base::Transform>(v2);

		const bx::Transform bx_t0 = MakeMatrix<bx::Transform>(v0);
		const bx::Transform bx_t1 = MakeMatrix<bx::Transform>(v1);
		const bx::Transform bx_t2 = MakeMatrix<bx::Transform>(v2);

		const bool b0 = Compare(glm_t0, bx_t0);
		const bool b1 = Compare(glm_t1, bx_t1);
		const bool b2 = Compare(glm_t2, bx_t2);

		assert(b0 && b1 && b2);

		glm_base::Transform glm_tmp;
		glm_tmp = glm_t2.data * glm_t1.data;
		glm_base::Transform glm_t3;
		glm_t3 = glm_t2.data * glm_t1.data * glm_t0.data;

		//std::cout << "glm_t3: \n" << std::endl;
		//glm_t3.Print();

		bx::Transform bx_tmp;
		bx::mtxMul(bx_tmp, bx_t0, bx_t1);
		bx::Transform bx_t3;
		bx::mtxMul(bx_t3, bx_tmp, bx_t2);

		bFlag = Compare(glm_t3, bx_t3, 1);

		assert(bFlag);
	}

}

void Test_Matrix_Scale()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto vScale = SerialGenerator(3);

		bx::Transform t0;
		glm_base::Transform t1;
		
		bx::mtxScale(t0, vScale[0], vScale[1], vScale[2]);
		glm_base::MatrixScale(t1, vScale[0], vScale[1], vScale[2]);

		bool bFlag = Compare(t0, t1);
		assert(bFlag);
	}
}

void Test_Matrix_Translate()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto vScale = SerialGenerator(3);

		bx::Transform t0;
		glm_base::Transform t1;

		bx::mtxTranslate(t0, vScale[0], vScale[1], vScale[2]);
		glm_base::MatrixTranslate(t1, vScale[0], vScale[1], vScale[2]);

		bool bFlag = Compare(t0, t1);
		assert(bFlag);
	}
}

void Test_Matrix_Transpose()
{
	for (size_t i = 0; i < G_INTENSITY; i++)
	{
		const auto v0 = SerialGenerator();

		const glm_base::Transform glm_t0 = MakeMatrix<glm_base::Transform>(v0);
		glm_base::Transform glm_t1;
		glm_base::MatrixTranspose(glm_t1, glm_t0);

		const bx::Transform bx_t0 = MakeMatrix<bx::Transform>(v0);
		bx::Transform bx_t1;
		bx::mtxTranspose(bx_t1, bx_t0);
		bool bFlag = Compare(glm_t1, bx_t1);
		assert(bFlag);
	}
}

void Test_Matrix_Identity()
{
	bool bFlag = true;
	const auto m = glm_base::MatrixIdentity();
	for (int i = 0; i < 4; i++)
	{
		bFlag &= m[i][i] == 1;
	}

	assert(bFlag);
}

void Test_Matrix_Lookat()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto veye =  SerialGenerator(3);
		const auto vat = SerialGenerator(3);
		const auto vup = SerialGenerator(3);

		bx::Transform t_bx;
		{
			const auto eye = MakeMatrix<bx::Vec3>(veye);
			const auto at = MakeMatrix<bx::Vec3>(vat);
			const auto up = MakeMatrix<bx::Vec3>(vup);

			bx::mtxLookAt(t_bx, eye, at, up);
		}
		
		glm_base::Transform t_glm;
		{
			const auto eye = MakeMatrix<glm::vec3>(veye);
			const auto at = MakeMatrix<glm::vec3 >(vat);
			const auto up = MakeMatrix<glm::vec3>(vup);

			t_glm = glm_base::MatrixLookAt(eye, at, up);
		}

		bool bFlag = true;
		bFlag = Compare(t_bx, t_glm, 1);
		assert(bFlag);
	}
}

void Test_Matrix_Rotate_Axis()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto v_angle = SerialGenerator(1);
		const auto angle = v_angle[0];
		const auto matrix_x1 = glm_base::MatrixRotateX(angle);
		const auto matrix_x2 = glm::rotate(glm::mat4{ 1 }, angle, glm::vec3{ 1.0f,0.0f,0.f });

		const auto matrix_y1 = glm_base::MatrixRotateY(angle);
		const auto matrix_y2 = glm::rotate(glm::mat4{ 1 }, angle, glm::vec3{ 0.0f,1.0f,0.f });
		
		const auto matrix_z1 = glm_base::MatrixRotateZ(angle);
		const auto matrix_z2 = glm::rotate(glm::mat4{ 1 }, angle, glm::vec3{ 0.0f,0.0f,1.f });

		const bool b1 = Compare(glm_base::Transform(matrix_x1), glm_base::Transform(matrix_x2));
		const bool b2 = Compare(glm_base::Transform(matrix_y1), glm_base::Transform(matrix_y2));
		const bool b3 = Compare(glm_base::Transform(matrix_z1), glm_base::Transform(matrix_z2));
		assert(b1 && b2 && b3);
	}
}

void Test_Matrix_RotateXYZ()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto vXYZ = SerialGenerator(3);

#if 0
		glm_base::Transform glm_t1;
		bx::Transform bx_t1;

		glm_base::MatrixRotateXYZ(glm_t1, vXYZ[0], vXYZ[1], vXYZ[2]);
		bx::mtxRotateXYZ(bx_t1, vXYZ[0], vXYZ[1], vXYZ[2]);

		glm_t1.Print();
		std::cout << std::endl;
		bx_t1.Print();
		bool bFlag = Compare(glm_t1, bx_t1);
#else
		glm_base::Transform glm_r_x{ glm_base::MatrixRotateX(vXYZ[0]) };
		glm_base::Transform glm_r_y{ glm_base::MatrixRotateY(vXYZ[1]) };
		glm_base::Transform glm_r_z{ glm_base::MatrixRotateZ(vXYZ[2]) };

		const auto glm_t0 = glm_base::Transform{ glm_r_z.data * glm_r_y.data * glm_r_x.data };
		glm_base::Transform glm_t1;
		glm_base::MatrixRotateZYX(glm_t1, vXYZ[0], vXYZ[1], vXYZ[2]);

		//std::cout << "glm_t0 from glm matrix multiply operation" << std::endl;
		//glm_t0.Print();

		//std::cout << "glm_t1 from glm_base::MatrixRotateXYZ function " << std::endl;
		//glm_t1.Print();

		bool bFlag = Compare(glm_t0, glm_t1);
#endif
		
		assert(bFlag);
	}
}

void Test_Matrix_InverseTranspose()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto v0 = SerialGenerator();

		const glm_base::Transform glm_t0 = MakeMatrix<glm_base::Transform>(v0);
		glm::mat4 glm_IVT = glm::inverseTranspose(glm_t0.data);

		const bx::Transform bx_t0 = MakeMatrix<bx::Transform>(v0);
		const float* modelMatrixData = bx_t0.data;
		glm::mat4 modelMatrix = glm::make_mat4(modelMatrixData);
		glm::mat4 bx_IVT = glm::inverseTranspose(modelMatrix);

		const glm_base::Transform glm_t1{ glm_IVT };

		std::vector<float> v1(16);
		std::copy(&bx_IVT[0][0], &bx_IVT[0][0] + 16, v1.begin());
		const bx::Transform bx_t1 = MakeMatrix<bx::Transform>(v1);
		//bx_t1.Print();	
		int a = 1;

		bool bFlag = Compare(glm_t1, bx_t1);
		assert(bFlag);
	}
}

void Test_Matrix_Make()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto v0 = SerialGenerator();

		const auto mtx0 =  glm::make_mat4(&v0[0]);
		
		const glm_base::Transform t0{ mtx0 };

		const auto t1 = MakeMatrix<bx::Transform>(v0);

		const bool bFlag = Compare(t0, t1);

		assert(bFlag);
	}
}

void Test_Matrix_Decompose()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto vec_translation = SerialGenerator(3);
		const auto vec_scale = PositiveSerialGenerator(3);
		bool bSkip = false;
		for (auto f : vec_scale) // 包含0的Scale会抹去对应维度的信息， 我称之为降维打击
		{
			if (glm::epsilonEqual(f, 0.0f, glm::epsilon<float>()))
			{
				bSkip = true;
				break;
			}
		}
		if (bSkip)
		{
			continue;
		}

		const auto vec_rotation = SerialGenerator(3);

		const auto mat_translation = glm_base::MatrixTranslate(vec_translation[0], vec_translation[1], vec_translation[2]);
		const auto mat_rotation = glm_base::MatrixRotateZYX(vec_rotation[0], vec_rotation[1], vec_rotation[2]);
		const auto mat_scale = glm_base::MatrixScale(vec_scale[0], vec_scale[1], vec_scale[2]);

		const auto mat_transform = mat_translation * mat_rotation * mat_scale;

		glm::vec3 transf_translation;
		glm::vec3 transf_scale;
		glm::vec3 transf_rotation;

		glm_base::MatrixDecompose(mat_transform, transf_translation, transf_rotation, transf_scale);

		const auto mat_rotation_from_decompose{ glm_base::MatrixRotateZYX(transf_rotation.x, transf_rotation.y, transf_rotation.z) };

		const bool b_t = Compare<std::vector<float>, glm::vec3, 3>(vec_translation, transf_translation);
		//const bool b_r = CompareRadian(vec_rotation, transf_rotation);
		const bool b_r = Compare(glm_base::Transform{ mat_rotation }, glm_base::Transform{ mat_rotation_from_decompose }, 0.01f);
		const bool b_s = Compare<std::vector<float>, glm::vec3, 3>(vec_scale, transf_scale, 0.001f);

		if (!(b_t && b_r && b_s))
		{
			assert(false);
			const auto r0 = glm_base::Transform{ mat_rotation };
			const auto r1 = glm_base::Transform{ mat_rotation_from_decompose };

#if 0
			std::cout << "r0: " << std::endl;
			r0.Print();

			std::cout << "r1: " << std::endl;
			r1.Print();
#endif

			int debug = 1;
			debug;
		}
		
	}
}

void Test_Matrix_Rotation_Decompose()
{
	auto XYZ = [](const float x, const float y, const float z) {
		const auto euler_x_matrix = glm::eulerAngleX(x);
		const auto euler_y_matrix = glm::eulerAngleY(y);
		const auto euler_z_matrix = glm::eulerAngleZ(z);
		return euler_x_matrix * euler_y_matrix * euler_z_matrix;
	};

	auto ZYX = [](const float x, const float y, const float z) {
		const auto euler_x_matrix = glm::eulerAngleX(x);
		const auto euler_y_matrix = glm::eulerAngleY(y);
		const auto euler_z_matrix = glm::eulerAngleZ(z);
		return euler_z_matrix * euler_y_matrix * euler_x_matrix;
	};

	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto vec_rotation = SerialGenerator(3);
		const auto rotation_angle = glm::vec3{ vec_rotation[0], vec_rotation[1], vec_rotation[2] };

		//  test glm xyz and zyx Euler Angle Order

		const auto matrix_xyz_raw = XYZ(rotation_angle.x, rotation_angle.y, rotation_angle.z);
		const auto matrix_xyz_glm = glm::eulerAngleXYZ(rotation_angle.x, rotation_angle.y, rotation_angle.z);
		const auto matrix_xyz_artificial = glm_base::MatrixRotateXYZ(rotation_angle.x, rotation_angle.y, rotation_angle.z);

		const auto matrix_zyx_raw = ZYX(rotation_angle.x, rotation_angle.y, rotation_angle.z);
		const auto matrix_zyx_glm = glm::eulerAngleZYX(rotation_angle.z, rotation_angle.y, rotation_angle.x);
		const auto matrix_zyx_artificial = glm_base::MatrixRotateZYX(rotation_angle.x, rotation_angle.y, rotation_angle.z);

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				assert(glm::epsilonEqual(matrix_xyz_raw[i][j], matrix_xyz_glm[i][j], 0.001f));
				assert(glm::epsilonEqual(matrix_xyz_raw[i][j], matrix_xyz_artificial[i][j], 0.001f));

				assert(glm::epsilonEqual(matrix_zyx_raw[i][j], matrix_zyx_glm[i][j], 0.001f));
				assert(glm::epsilonEqual(matrix_zyx_raw[i][j], matrix_zyx_artificial[i][j], 0.001f));
			}

		// test glm extract Euler Angle

		float x, y, z;
		glm::extractEulerAngleXYZ(matrix_xyz_raw, x, y, z);
		const auto artificial_matrix_xyz = XYZ(x, y, z);

		glm::extractEulerAngleZYX(matrix_zyx_raw, z, y, x);
		const auto artificial_matrix_zyx = ZYX(x, y, z);

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				assert(glm::epsilonEqual(matrix_xyz_raw[i][j], artificial_matrix_xyz[i][j], 0.001f));
				assert(glm::epsilonEqual(matrix_zyx_raw[i][j], artificial_matrix_zyx[i][j], 0.001f));
			}
	}
}

void Test_Quaternion_FromMatrix()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto vec_rotation = SerialGenerator(3);
		const auto vec_translation = SerialGenerator(3);
		const auto rotation_angle = glm::vec3{vec_rotation[0], vec_rotation[1], vec_rotation[2] };

		const auto euler_matrix = glm::eulerAngleZYX(rotation_angle.z, rotation_angle.y, rotation_angle.x);

		const auto q = glm::quat{ rotation_angle }; // euler rotation to quaterion
		const auto q1 = glm::quat{ euler_matrix }; // rotation matrix to quaternion

		/*
		Note:
			It's not consistent that the quaternion constructs from rotation matrix compared to original quaternion which forms the input rotation matrix.
		But, both the quaternion(original and constructed from rotation matrix) can construct a consistent rotation matrix
		*/

		//const auto q_equal = glm::epsilonEqual(q, q1, 0.0001f);
		//assert(q_equal.x && q_equal.y && q_equal.z && q_equal.w);

		const auto q_matrix = glm::mat4{ q };
		const auto q_matrix1 = glm::mat4{ q1 };

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				assert(glm::epsilonEqual(euler_matrix[i][j], q_matrix[i][j], 0.001f));
				assert(glm::epsilonEqual(euler_matrix[i][j], q_matrix1[i][j], 0.001f));
			}
	}
}

namespace {
	glm::mat4 CustomRotateMatrix(const glm::quat& q)
	{
		const float x = q.x * 2.0f;
		const float y = q.y * 2.0f;
		const float z = q.z * 2.0f;
		const float xx = q.x * x;
		const float yy = q.y * y;
		const float zz = q.z * z;
		const float xy = q.x * y;
		const float xz = q.x * z;
		const float yz = q.y * z;
		const float wx = q.w * x;
		const float wy = q.w * y;
		const float wz = q.w * z;

		glm::mat4 m{1};
		m[0][0] = 1.0f - (yy + zz); m[0][1] = xy + wz; m[0][2] = xz - wy; m[0][3] = 0.0f;
		m[1][0] = xy - wz; m[1][1] = 1.0f - (xx + zz); m[1][2] = yz + wx; m[1][3] = 0.0f;
		m[2][0] = xz + wy; m[2][1] = yz - wx; m[2][2] = 1.0f - (xx + yy); m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
		return m;
	}
}

void Test_Quaternion_ToMatrix()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto vec_quat = SerialGenerator(4);
		const auto q = glm::quat{ vec_quat[0], vec_quat[1], vec_quat[2], vec_quat[3] };
		const auto m0 = CustomRotateMatrix(q);
		const auto m1 = glm::mat4{ q };
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				assert(glm::epsilonEqual(m0[i][j], m1[i][j], 0.001f));
			}
	}
}

void Test_Quaternion2Euler()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto vec_quat = SerialGenerator(4);
		const auto q = glm::normalize(glm::quat{ vec_quat[0], vec_quat[1], vec_quat[2], vec_quat[3] });

		const auto matrix_q = glm::mat4{ q };

		float x, y, z;
		glm::extractEulerAngleXYZ(matrix_q, x, y, z);
		const auto matrix_euler_xyz = glm::eulerAngleXYZ(x, y, z);
		glm::extractEulerAngleZYX(matrix_q, z, y, x);
		const auto matrix_euler_zyx = glm::eulerAngleZYX(z, y, x);

		bool flag_xyz = true;
		bool flag_zyx = true;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				flag_xyz = flag_xyz && (glm::epsilonEqual(matrix_q[i][j], matrix_euler_xyz[i][j], 0.0001f));
				flag_zyx = flag_zyx && (glm::epsilonEqual(matrix_q[i][j], matrix_euler_zyx[i][j], 0.0001f));
			}

		assert(flag_xyz && flag_zyx);
	}

}

void Test_Euler2Quaternion()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto vec_eulerAngles = SerialGenerator(3);
		const auto eulerAngles = glm::vec3{ vec_eulerAngles[0], vec_eulerAngles[1], vec_eulerAngles[2] };
		const auto q = glm::quat{ eulerAngles };
		
		const auto matrix_q = glm::mat4{ q };
		//const auto matrix_euler = glm::eulerAngleXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);
		const auto matrix_euler = glm::eulerAngleZYX(eulerAngles.z, eulerAngles.y, eulerAngles.x);

		// this case tells us that the rotation matrix from Quternion is equivalent to the rotation matrix from ZYX rotation matrix

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				assert(glm::epsilonEqual(matrix_q[i][j], matrix_euler[i][j], 0.0001f));
			}

	}
}

void Test_Matrix_GLM_EulerMatrix()
{
	for (int i = 0; i < G_INTENSITY; i++)
	{
		const auto vec_eulerAngles = SerialGenerator(3);

		auto ZYX = [](float x, float y, float z) {
			const auto euler_x = glm::eulerAngleX(x);
			const auto euler_y = glm::eulerAngleY(y);
			const auto euler_z = glm::eulerAngleZ(z);
			return euler_z * euler_y * euler_x;
		};

		auto XYZ = [](float x, float y, float z) {
			const auto euler_x = glm::eulerAngleX(x);
			const auto euler_y = glm::eulerAngleY(y);
			const auto euler_z = glm::eulerAngleZ(z);
			return euler_x * euler_y * euler_z;
		};

		const auto m0 = XYZ(vec_eulerAngles[0], vec_eulerAngles[1], vec_eulerAngles[2]);
		const auto m1 = glm::eulerAngleXYZ(vec_eulerAngles[0], vec_eulerAngles[1], vec_eulerAngles[2]);

		const auto m2 = ZYX(vec_eulerAngles[0], vec_eulerAngles[1], vec_eulerAngles[2]);
		const auto m3 = glm::eulerAngleZYX(vec_eulerAngles[2], vec_eulerAngles[1], vec_eulerAngles[0]);

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				assert(glm::epsilonEqual(m0[i][j], m1[i][j], 0.001f));
				assert(glm::epsilonEqual(m2[i][j], m3[i][j], 0.001f));
			}

	}
}

