#include "vec2.h"

#include <cmath>

namespace ks
{
	void Vec2::Normalize()
	{
		float length = Length();
		if (length != 0.f)
		{
			X /= length;
			Y /= length;
		}
	}

	float Vec2::Length()
	{
		return sqrt((X * X) + (Y * Y));
	}

	Vec2& Vec2::operator+=(const Vec2& vec)
	{
		X += vec.X;
		Y += vec.Y;
		return *this;
	}

	Vec2 operator-(const Vec2& left, const Vec2& right)
	{
		return Vec2
		{
			left.X - right.X,
			left.Y - right.Y
		};
	}

	Vec2 operator*(const Vec2& vec, float32 scalar)
	{
		return Vec2 { vec.X * scalar, vec.Y * scalar };
	}
}