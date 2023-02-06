#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include "../types.h"

namespace ks
{
	LPWSTR Win32UTF8ToUTF16(stringUTF8 string);
}