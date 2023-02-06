#pragma once

#include "renderer.h"

namespace ks
{
	enum class TextureFormat
	{
		RGBA32,
		R8
	};

	namespace Texture
	{
		TextureHandle Make(uint32 width, uint32 height, const byte* data,
			TextureFormat format = TextureFormat::RGBA32);

		void Bind(TextureHandle handle);
	}
}