#pragma once
#ifdef KS_WIN32
#include "Win32/win32_keycodes.h"
#else
#error NO KEYCODES FOR CURRENT PLATFORM
#endif

namespace ks
{
	enum class MouseKeyCodes
	{
		LeftButton = 0
	};
}