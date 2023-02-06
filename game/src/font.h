#pragma once


#include "../vendor/stb/stb_truetype.h"
#include "platform/texture.h"

namespace ks
{
	struct Font
	{
		static Font* Load(ks::stringUTF8 path, float size);

		ks::TextureHandle Bitmap;
		stbtt_bakedchar GlyphInfo[96];
	};
}