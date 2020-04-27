#pragma once
#include "GLM_Transform.h"
#include "BX_Transform.h"
#include <vector>

template<typename T>
T MakeMatrix(const std::vector<float>& v)
{
	T t;
	std::copy(v.begin(), v.end(), &t[0]);
	return t;
}

template<typename T>
std::vector<float> MakeVectorFromMatrix(const T& t)
{
	std::vector<float> v(16);
	std::copy(&t[0], &t[16], v.begin());
	return v;
}

template<typename T0, typename T1>
void Copy(const T0& t0, T1& t1)
{
	for (size_t i = 0; i < 16; i++)
	{
		t1[i] = t0[i];
	}
}

template<typename T0, typename T1, size_t L=16>
bool Compare(const T0& t0, const T1& t1, const float bias = glm::epsilon<float>())
{
	bool bFlag = true;
	for (size_t i = 0; i < L; i++)
	{
		bFlag &= std::abs(t0[i] - t1[i]) < bias;
	}

	return bFlag;
}

template<typename T0, typename T1, size_t L = 3>
bool CompareRadian(const T0& t0, const T1& t1)
{
	bool bFlag = true;
	for (size_t i = 0; i < L; i++)
	{
		bFlag &= CompareRadian(t0[i], t1[i]);
	}

	return bFlag;
}

template<>
bool CompareRadian(const float& radian0, const float& radian1);

template<typename T0, typename T1>
bool Compare(const T0& t0, const T1& t1, int)
{
	bool bFlag = true;
	for (size_t i = 0; i < 16; i++)
	{
		bFlag &= std::abs(t0[i] - t1[i]) < glm::epsilon<float>();
		if (!bFlag)
		{
			assert(false);
		}
	}

	return bFlag;
}

// [min, max)
std::vector<float> RangeGenerator(float min, float max, const size_t len = 16);

std::vector<float> SerialGenerator(const size_t len = 16);

std::vector<float> PositiveSerialGenerator(const size_t len = 16);

void Test_Matrix_Mul();

void Test_Matrix_Scale();

void Test_Matrix_Translate();

void Test_Matrix_Transpose();

void Test_Matrix_Identity();

void Test_Matrix_Lookat();

void Test_Matrix_Rotate_Axis();

void Test_Matrix_RotateXYZ();

void Test_Matrix_InverseTranspose();

void Test_Matrix_Make();

void Test_Matrix_Decompose();

void Test_Matrix_Rotation_Decompose();

void Test_Quaternion_FromMatrix();

void Test_Quaternion_ToMatrix();

void Test_Quaternion2Euler();

void Test_Euler2Quaternion();

void Test_Matrix_GLM_EulerMatrix();

void Test_GimbalLock();
