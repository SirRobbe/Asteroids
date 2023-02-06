#include "../vertexarray.h"

#include <glad/glad.h>

namespace ks::VertexArray
{
	void Bind(VertexArrayHandle handle)
	{
		glBindVertexArray(handle);
	}

	void ApplyVertexBufferLayout(VertexArrayHandle handle,
		const VertexBufferLayout& layout)
	{
		for(size_t i = 0; i < layout.ElementCount; i++)
		{
			glVertexAttribPointer(layout.pAttributeIDs[i], layout.pCounts[i], GL_FLOAT,
				layout.pNormalizes[i], layout.Stride,
				reinterpret_cast<void*>(layout.pOffsets[i]));

			glEnableVertexAttribArray(layout.pAttributeIDs[i]);
		}
	}

	void Release(VertexArrayHandle handle)
	{
		glDeleteVertexArrays(1, &handle);
	}
}
