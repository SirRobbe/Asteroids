#include "font.h"

#include "platform/file.h"
#include "data_structures/fixed_array.h"

namespace ks
{
	Font* Font::Load(ks::stringUTF8 path, float size)
	{
		Font* font = new Font{};
		ks::FixedArray<ks::byte>* ttfData = ks::File::ReadAllBytes(path);
		ks::byte* tmpBitmap = new ks::byte[512 * 512];
        
		stbtt_BakeFontBitmap(ttfData->Data(), 0, size, tmpBitmap, 512, 512, 32, 96,
                             font->GlyphInfo);
		font->Bitmap = ks::Texture::Make(512, 512, tmpBitmap, ks::TextureFormat::R8);
        
		delete ttfData;
		delete[] tmpBitmap;
		return font;
	}
}
