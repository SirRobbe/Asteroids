#include "../texture.h"

#include "../Win32/kswindows.h"
#include <glad/glad.h>
#include <cassert>

namespace ks::Texture
{
	TextureHandle Make(uint32 width, uint32 height, const byte* data,
		TextureFormat format)
	{
		GLint textureFormat = 0;
		GLint internalFormat = 0;
		switch (format)
		{
			case TextureFormat::RGBA32:
			{
				textureFormat = GL_RGBA;
				internalFormat = GL_RGBA;
				break;
			}
			case TextureFormat::R8:
			{
				textureFormat = GL_R8;
				internalFormat = GL_RED;
				break;
			}
			default:
			{
				assert(false);
				break;
			}
		}

		TextureHandle handle = 0;
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, internalFormat,
			GL_UNSIGNED_BYTE, data);

		return handle;
	}

	void Bind(TextureHandle handle)
	{
		glBindTexture(GL_TEXTURE_2D, handle);

	}
}