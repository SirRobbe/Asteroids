#include "matrix4x4.h"

namespace ks
{
	Matrix4x4 Matrix4x4::Orthographic(float32 left, float32 top, float32 right,
		float32 bottom, float32 near, float32 far)
	{
		Matrix4x4 matrix = {};
		matrix.Elements[0] = 2.f / (right - left);
		matrix.Elements[3] = -((right + left) / (right - left));
		matrix.Elements[5] = 2.f / (top - bottom);
		matrix.Elements[7] = -((top + bottom) / (top - bottom));
		matrix.Elements[10] = -(2.f / (far - near));
		matrix.Elements[11] = -((far + near) / (far - near));
		matrix.Elements[15] = 1.f;

		return matrix;
	}
}