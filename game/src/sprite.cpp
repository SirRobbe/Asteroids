#include "sprite.h"

#include "vertexbufferlayout.h"
#include "platform/vertexarray.h"
#include "platform/texture.h"

// TODO: maybe abstract stb away in the platform independence layer
#ifdef KS_WIN32
#define STBI_WINDOWS_UTF8
#endif
#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace ks
{
	Sprite Sprite::LoadFromFile(stringUTF8 path, RenderState * pState)
	{
		Sprite sprite = {};

		int width = 0;
		int height = 0;
		int requiredChannels = 0;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* pData = stbi_load(path, &width, &height, &requiredChannels, 4);

		sprite.Texture = Texture::Make(width, height, (byte*)pData);
		stbi_image_free(pData);

		sprite.VertexArray = ks::Renderer::CreateVertexArray(*pState);
		VertexArray::Bind(sprite.VertexArray);

		// PERFORMANCE: Instead of having a own vertexbuffer for every Sprite it would be much faster to
		// to have one vertexbuffer and write in the geometry dynamically
		float vertices[] =
		{
			-width / 2, height / 2, 0.f, 0.f,
			-width / 2, -height / 2, 0.f, 1.f,
			width / 2, -height / 2, 1.f, 1.f,

			-width / 2, height / 2, 0.f, 0.f,
			width / 2, -height / 2, 1.f, 1.f,
			width / 2, height / 2, 1.f, 0.f
		};

		VertexBufferHandle spriteVB = 
			Renderer::CreateVertexBuffer(*pState, vertices, sizeof(vertices));

		VertexBufferLayoutElement elements[] =
		{
			{0, false, 2},
			{1, false, 2}
		};
		VertexBufferLayout layout(elements, 2);
		VertexArray::ApplyVertexBufferLayout(sprite.VertexArray, layout);

		sprite.Width = width;
		sprite.Height = height;
		return sprite;
	}
}
