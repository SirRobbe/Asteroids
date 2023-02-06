#include "random.h"
// TODO: Write custom assert
#include <cassert>


namespace ks::Random
{
	INTERNAL uint64 XorShift64()
	{
		LOCAL_GLOBAL uint64 x = 88172645463325252LL;
		x ^= (x << 13);
		x ^= (x >> 7);
		return (x ^= (x << 17));
	}

	int Range(int min, int max)
	{
		assert(min <= max);
		int range = max - min + 1;
		return (XorShift64() % range) + min;
	}
}