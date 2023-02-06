#pragma once

#include "../vertexbufferlayout.h"
#include "renderer.h"

namespace ks::VertexArray
{
	void Bind(VertexArrayHandle handle);
	void ApplyVertexBufferLayout(VertexArrayHandle handle, 
		const VertexBufferLayout& layout);
	void Release(VertexArrayHandle handle);
}