#pragma once

#include "../platform/types.h"

namespace ks
{
	struct Vec2 final
	{
		void Normalize();
		float Length();

		Vec2& operator+=(const Vec2& vec);

		float32 X;
		float32 Y;
	};

	Vec2 operator-(const Vec2& left, const Vec2& right);
	Vec2 operator*(const Vec2& vec, float32 scalar);
}