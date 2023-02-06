#pragma once

#include "../platform/types.h"
#include "vec2.h"

namespace ks
{
	struct Matrix3x3
	{
		static Matrix3x3 Scale(float32 x, float32 y);
		static Matrix3x3 Scale(float32 uniformScale);
		static Matrix3x3 Scale(const Vec2& scale);

		static Matrix3x3 Rotation(float32 rotation);

		static Matrix3x3 Translation(float32 x, float32 y);
		static Matrix3x3 Translation(const Vec2& translation);

		float32 Elements[9];
	};

	Matrix3x3 operator*(const Matrix3x3& left, const Matrix3x3& right);
}