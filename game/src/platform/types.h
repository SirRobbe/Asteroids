#pragma once

#include <cstdint>

namespace ks
{
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

	using int32 = std::int32_t;
	using int64 = std::int64_t;

	using float32 = float;

	using byte = unsigned char;
	using char8 = char;
	using char16 = char16_t;

	using char32 = char32_t;

	using string = char8*;
	using stringUTF8 = char8* ;
	using stringUTF16 = char16*;

}