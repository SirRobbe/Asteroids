#pragma once

#include "../platform/types.h"

namespace ks
{
	struct Matrix4x4
	{
		static Matrix4x4 Orthographic(float32 left, float32 top, float32 right,
			float32 bottom, float32 near, float32 far);

		float32 Elements[16];
	};


}