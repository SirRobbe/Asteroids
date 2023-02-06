#pragma once

#include <vector>
#include "platform/types.h"


namespace ks
{
	struct VertexBufferLayoutElement final
	{
		uint32 AttributeID;
		bool Normalize;
		uint32 Count;
	};

	struct VertexBufferLayout final
	{
		VertexBufferLayout(const VertexBufferLayoutElement* pElements, size_t count);
		~VertexBufferLayout();

		uint32* pAttributeIDs;
		bool* pNormalizes;
		uint32* pCounts;
		uint32* pOffsets;

		size_t ElementCount;
		uint32 Stride;
	};
}