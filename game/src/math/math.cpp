#include "math.h"

namespace ks::Math
{
	float Clamp(float value, float min, float max)
	{
		if(value < min)
		{
			return min;
		}
		else if(value > max)
		{
			return max;
		}
		else
		{
			return value;
		}
	}
}