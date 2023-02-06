#pragma once

#include "platform/types.h"

namespace ks
{
	struct Color final
	{
		static Color WHITE;
		static Color BLACK;

		union
		{
			struct
			{
				float32 R;
				float32 G;
				float32 B;
				float32 A;
			};

			float32 Values[4];
		};
	};

	Color operator*(const Color& color, float scalar);
	Color operator*(float scalar, const Color& color);
	Color operator+(const Color& left, const Color& right);
}