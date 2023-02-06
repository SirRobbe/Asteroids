#pragma once

#include "types.h"
#include "renderer.h"

namespace ks
{
	namespace VertexBuffer
	{

		void Bind(VertexBufferHandle handle);
		void InitData(const float32* pData, size_t size, VertexBufferUsage accessMode);
		void UpdateData(const float32* data, size_t size, size_t offset);
		void Release(VertexBufferHandle handle);
	}
}