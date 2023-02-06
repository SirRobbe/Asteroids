#include "matrix3x3.h"

#include <cmath>

namespace ks
{
	Matrix3x3 Matrix3x3::Scale(float32 x, float32 y)
	{
		Matrix3x3 mat = {};
		mat.Elements[0] = x;
		mat.Elements[4] = y;
		mat.Elements[8] = 1.f;
		return mat;
	}

	Matrix3x3 Matrix3x3::Scale(float32 uniformScale)
	{
		return Matrix3x3::Scale(uniformScale, uniformScale);
	}

	Matrix3x3 Matrix3x3::Scale(const Vec2& scale)
	{
		return Scale(scale.X, scale.Y);
	}

	Matrix3x3 Matrix3x3::Rotation(float32 rotation)
	{
		Matrix3x3 mat = {};
		mat.Elements[0] = cos(rotation);
		mat.Elements[1] = sin(rotation);
		mat.Elements[3] = -sin(rotation);
		mat.Elements[4] = cos(rotation);
		mat.Elements[8] = 1.f;
		return mat;
	}

	Matrix3x3 Matrix3x3::Translation(float32 x, float32 y)
	{
		Matrix3x3 mat = {};
		mat.Elements[0] = 1.f;
		mat.Elements[2] = x;
		mat.Elements[4] = 1.f;
		mat.Elements[5] = y;
		mat.Elements[8] = 1.f;

		return mat;
	}

	Matrix3x3 Matrix3x3::Translation(const Vec2& translation)
	{
		return Translation(translation.X, translation.Y);
	}

	Matrix3x3 operator*(const Matrix3x3& left, const Matrix3x3& right)
	{
		Matrix3x3 mat = {};

		// INCOMPLETE: Replace this with dotproduct of row and column when vec3 is implemented
		for(ks::int32 row = 0; row < 3; row++)
		{
			for(ks::int32 column = 0; column < 3; column++)
			{
				mat.Elements[row * 3 + column] = left.Elements[row * 3] * right.Elements[column] + 
					left.Elements[row * 3 + 1] * right.Elements[column + 3] +
					left.Elements[row * 3 + 2] * right.Elements[column + 6];
			}
		}

		return mat;
	}
}

