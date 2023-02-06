#pragma once

#include "platform/renderer.h"
#include "platform/types.h"

namespace ks
{
	struct Sprite
	{
		static Sprite LoadFromFile(stringUTF8 path, RenderState* pState);

		ks::TextureHandle Texture;
		ks::VertexArrayHandle VertexArray;
		ks::uint32 Width;
		ks::uint32 Height;

	};
}