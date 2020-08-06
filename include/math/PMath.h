#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

const float kEpsilon = 0.00001f;

class PMath
{
public:
	static glm::mat4 TranslationMatrix(const glm::vec3& translation);

	static glm::mat4 ScaleMatrix(const float scale);

	/**
	* @brief Construct a local coordinate-system of which the z-axis is the input parameter "z_axis"
	* @returns glm::mat3 A Matrix of which the column[0] is x-axis, the column[1] is y-axis and the column[2] is z-axis
	* @param z_axis A vector that indicates the local coordinate-system's z-axis
	* @param _up A vector that indicates the local coordinate-system's up orientation direction, do't need to be the y-axis
	*/
	static glm::mat3 LookAtMatrix(const glm::vec3& z_axis, const glm::vec3& _up);

	/**
	* @brief Use the X->Y->Z rotation order around the fixed world coordinate system (Extrinsic rotation) to construct a rotation matrix.
	*	On the other hand, using the same X->Y->Z order, but the coordinate system is based on the previous rotation, can also construct
	*	a rotation matrix, of which we call it intrinsic rotation. The intrinsic rotation order is the reverse of extrinsic rotation order.
	*	So, if you want to get a X->Y->Z order intrinsic rotation, use Z->Y->X order extrinsic rotation instead.
	*	As GWEngine uses column-major matrix, a transform matrix is a left-operand. In this way, this function actually yields a product of
	*	MatrixRotateEulerZ *  MatrixRotateEulerY * MatrixRotateEulerX.
	* @param _ax The Euler angle around x axis which is the fixed world coordinate system
	* @param _ay The Euler angle around y axis which is the fixed world coordinate system
	* @param _az The Euler angle around z axis which is the fixed world coordinate system
	*/
	static glm::mat4 MatrixRotateFromExtrinsicXYZ(float _ax, float _ay, float _az);

	/**
	* @brief Use the Z->Y->X rotation order around the fixed world coordinate system (Extrinsic rotation) to construct a rotation matrix.
	*	As GWEngine uses column-major matrix, a transform matrix is a left-operand. In this way, this function actually yields a product of
	*	MatrixRotateEulerX *  MatrixRotateEulerY * MatrixRotateEulerZ.
	*	(More details, see @ref GWMath::MatrixRotateFromExtrinsicXYZ)
	* @param _ax The Euler angle around x axis which is the fixed world coordinate system
	* @param _ay The Euler angle around y axis which is the fixed world coordinate system
	* @param _az The Euler angle around z axis which is the fixed world coordinate system
	*/
	static glm::mat4 MatrixRotateFromExtrinsicZYX(float _ax, float _ay, float _az);

	/**
	*@brief  Decompose a transform matrix to three elements: translation, rotation(Euler Angles), scale.
		Limits:
			1. All original scale components' sign should be consistent (all positve or negative).
			2. To extract original scale which contain a zero component is impossible.
			3. The original rotation should be X->Y->Z order extrinsic (Matrix multiplication order is Z * Y *X)
	* @parameter t Input transform matrix
	* @parameter translation  Output translation (plain meaning)
	* @parameter rotation X->Y->Z order extrinsic rotation
	*	In 3D space, one rotation matrix can be combined with different rotations.
	* This can give rise to a curious situation of which the result is totally different from the original input rotations.
	* Anyway, we just make sure that the final rotation matrix is exactly same.
	* @parameter scale Output scale (plain meaning)
	*/
	static void TransformMatrixDecompose(const glm::mat4& t, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

	/**
	* @brief Decompose a transform matrix to three elements: translation, rotation(Quaternion), scale.
	*/
	static void TransformMatrixDecompose(const glm::mat4& t, glm::vec3& translation, glm::quat& q, glm::vec3& scale);

	/**
	* @brief Extract the translation from a transform matrix
	*/
	static glm::vec3 ExtractTranslation(const glm::mat4& t);

	/*
	* @brief Extract the scale from a transform matrix
	*/
	static glm::vec3 ExtractScale(const glm::mat4& t);

	/**
	* @brief Normalize a vector
	*/
	static void NormalizeVector(const glm::vec3& vector, glm::vec3& out);

	// ¡ý Rotation & Quaternion

	/**
	* @brief Get a quaternion which can be used to rotate v0 to v1
	* @returns glm::quat Yield a quaternion which can rotate v0 to v1
	*/
	static glm::quat VectorRotate(const glm::vec3& v0, const glm::vec3& v1);

	/**
	* @brief Build a quaternion created an axis vector and an angle. The vector must be normalized!
	* @param angleRad The angle to rotate
	* @param v The vector around with
	*/
	static glm::quat QuaternionRotate(const float angleRad, const glm::vec3& v);

};

