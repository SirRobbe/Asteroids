#include "../clock.h"

#include "kswindows.h"

namespace ks::Clock
{
	// Returns ellapsed time in ms
	int32 Now()
	{
		LARGE_INTEGER frequency = {};
		QueryPerformanceFrequency(&frequency);
		LARGE_INTEGER now = {};
		QueryPerformanceCounter(&now);
		return (1000 * now.QuadPart) / frequency.QuadPart;
	}
}