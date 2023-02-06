#include "vertexbufferlayout.h"

namespace ks
{
	VertexBufferLayout::VertexBufferLayout(const VertexBufferLayoutElement* pElements,
		size_t count) : ElementCount(count), Stride(0)
	{
		pAttributeIDs = new uint32[ElementCount];
		pNormalizes = new bool[ElementCount];
		pCounts = new uint32[ElementCount];
		pOffsets = new uint32[ElementCount];

		for(size_t i = 0; i < ElementCount; i++)
		{
			pOffsets[i] = Stride;
			pAttributeIDs[i] = pElements[i].AttributeID;
			pNormalizes[i] = pElements[i].Normalize;
			pCounts[i] = pElements[i].Count;
			Stride += pElements[i].Count * sizeof(float32);
		}
	}

	VertexBufferLayout::~VertexBufferLayout()
	{
		delete[] pAttributeIDs;
		delete[] pNormalizes;
		delete[] pCounts;
		delete[] pOffsets;
	}
}
