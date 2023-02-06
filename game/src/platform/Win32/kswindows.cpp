#include "kswindows.h"
#include <cassert>
namespace ks
{
	LPWSTR Win32UTF8ToUTF16(stringUTF8 string)
	{
		int requiredLenght = MultiByteToWideChar(CP_UTF8, 0, string, -1, nullptr, 0);
		if(requiredLenght == 0)
		{
			// TODO: Diagnostics ...
			assert(false);
			return nullptr;
		}

		LPWSTR utf16String = new WCHAR[requiredLenght];
		if (MultiByteToWideChar(CP_UTF8, 0, string, -1, utf16String, requiredLenght) == 0)
		{
			// TODO: Diagnostics ...
			assert(false);
			return nullptr;
		}

		return utf16String;
	}
}