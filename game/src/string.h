#pragma once

#include "platform/types.h"
#include "data_structures/fixed_array.h"

namespace ks
{
	class String final
	{
	public:
		String(const char* string);

		int64 NextUTF8Character(int64 index);
		char32 GetUTFCharacter(int64 index);

		int64 Size();
		int64 Length();

		byte operator[](int64 index);
	private:
		ks::FixedArray<ks::byte> Data;
	};
}

