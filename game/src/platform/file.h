#pragma once

#include "types.h"
#include "../data_structures/fixed_array.h"

namespace ks::File
{
	string ReadAllText(stringUTF8 path);
	FixedArray<byte>* ReadAllBytes(stringUTF8 path);
}