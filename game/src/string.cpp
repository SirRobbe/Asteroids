#include "string.h"
#include <string.h>

#include <cassert>

namespace ks
{
	String::String(const char* string) :
		Data(strlen(string))
	{
		for(int i = 0; i < Data.Count; i++)
		{
			Data[i] = string[i];
		}
	}

	int64 String::NextUTF8Character(int64 index)
	{
		assert(index >= 0 && index < Data.Count);
		if ((Data[index] & (1 << 7)) == 0)
		{
			return index + 1;
		}
		else if ((Data[index] & (1 << 5)) == 0)
		{
			return index + 2;
		}
		else if ((Data[index] & (1 << 4)) == 0)
		{
			return index + 3;
		}
		else
		{
			return index += 4;
		}
	}

	char32  String::GetUTFCharacter(int64 index)
	{
		char32 character = 0;
		assert(index >= 0 && index < Data.Count);
		if ((Data[index] & (1 << 7)) == 0)
		{
			character = Data[index];
		}
		else if ((Data[index] & (1 << 5)) == 0)
		{
			character |= Data[index] << 8;
			character |= Data[index + 1];
		}
		else if ((Data[index] & (1 << 4)) == 0)
		{
			character |= Data[index] << 16;
			character |= Data[index + 1] << 8;
			character |= Data[index + 2];
		}
		else
		{
			character |= Data[index] << 24;
			character |= Data[index + 1] << 16;
			character |= Data[index + 2] << 8;
			character |= Data[index + 3];
		}

		return character;
	}

	int64 String::Size()
	{
		return Data.Count;
	}

	int64 String::Length()
	{
		int length = 0;
		int i = 0;
		while(i < Data.Count)
		{
			length++;
			if ((Data[i] & (1 << 7)) == 0)
			{
				i++;
			}
			else if ((Data[i] & (1 << 5)) == 0)
			{
				i += 2;
			}
			else if ((Data[i] & (1 << 4)) == 0)
			{
				i += 3;
			}
			else
			{
				i += 4;
			}
		}
		return length;
	}

	byte String::operator[](int64 index)
	{
		assert(index >= 0 && index < Data.Count);
		return Data[index];
	}
}
