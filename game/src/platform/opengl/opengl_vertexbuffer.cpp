#include "../vertexbuffer.h"

#include "glad/glad.h"
#include <cassert>

namespace ks::VertexBuffer
{
	void Bind(VertexBufferHandle handle)
	{
		glBindBuffer(GL_ARRAY_BUFFER, handle);
	}

	void InitData(const float32* pData, size_t size, VertexBufferUsage usage)
	{
		GLint usageType = 0;
		switch(usage)
		{
			case VertexBufferUsage::Static: {usageType = GL_STATIC_DRAW; break; }
			case VertexBufferUsage::Dynamic: {usageType = GL_DYNAMIC_DRAW; break; }
			default: {assert(false); break; }
		}

		glBufferData(GL_ARRAY_BUFFER, size, pData, usageType);
	}

	void UpdateData(const float32* data, size_t size, size_t offset)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	void Release(VertexBufferHandle handle)
	{
		glDeleteBuffers(1, &handle);
	}
}