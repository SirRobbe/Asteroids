#include "color.h"
#include "math/math.h"

namespace ks
{
	Color Color::WHITE = Color{ 1, 1, 1, 1 };
	Color Color::BLACK = Color{ 0, 0, 0, 1 };

	Color operator*(const Color& color, float scalar)
	{
		return Color
		{
			color.R * scalar,
			color.G * scalar,
			color.B * scalar,
			color.A * scalar
		};
	}

	Color operator*(float scalar, const Color& color)
	{
		return color * scalar;
	}

	Color operator+(const Color& left, const Color& right)
	{
		return Color
		{
			Math::Clamp(left.R + right.R, 0, 1),
			Math::Clamp(left.G + right.G, 0, 1),
			Math::Clamp(left.B + right.B, 0, 1),
			Math::Clamp(left.A + right.A, 0, 1),
		};
	}
}