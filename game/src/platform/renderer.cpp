#include "renderer.h"
#include "shader.h"
#include "vertexbuffer.h"
#include "vertexarray.h"
#include "../font.h"
#include "../vendor/stb/stb_truetype.h"

namespace ks
{
    void TextRenderer::RenderText(TextRenderer* renderer, float x, float y,
								  const char* text,
								  Font* font,
								  Color color)
    {
		ks::Texture::Bind(font->Bitmap);
        ks::Shader::Use(renderer->Shader);
        ks::Shader::SetUniform(renderer->Shader, (ks::string)"uProjection", *renderer->Projection);
        ks::Shader::SetUniform(renderer->Shader, (ks::string)"uColor", color);
        ks::VertexArray::Bind(renderer->VertexArray);
        ks::VertexBuffer::Bind(renderer->VertexBuffer);
        while (*text)
        {
            // TODO(Fabian): Currently we only support default ASCII Values change
            // this later to support a wider range
            if(*text >= 32 && *text < 128)
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(font->GlyphInfo, 512, 512, *text - 32, &x, &y, &q, 1);//1=opengl & d3d10+,0=d3d9
                float data[] =
                {
                    // TODO: Currently i changed the t0s to t1 and vice versa since the image is not flipped
                    q.x0, q.y0, q.s0, q.t0,
                    q.x1, q.y0, q.s1, q.t0,
                    q.x1, q.y1, q.s1, q.t1,
                    q.x0, q.y1, q.s0, q.t1,
                    q.x0, q.y0, q.s0, q.t0,
                    q.x1, q.y1, q.s1, q.t1
                };
                
                ks::VertexBuffer::Bind(renderer->VertexBuffer);
                ks::VertexBuffer::UpdateData(data, sizeof(data), 0);
                ks::DrawElements(6);
            }
            ++text;
        }
    }

	void TextRenderer::RenderTextCentered(TextRenderer* renderer, float x, float y, const char* text, Font* font, Color color)
	{
		int textWidth = 0;
		int textHeight = 0;
		float tmpX = x;
		float tmpY = y;
		stbtt_aligned_quad glyph;
		for (const char* letter = text; *letter; letter++)
		{
			stbtt_GetBakedQuad(font->GlyphInfo, 512, 512, *letter - 32, &tmpX, &tmpY, &glyph, 1);
			textWidth += glyph.x1 - glyph.x0;
			int letterHeight = glyph.y1 - glyph.y0;
			if(letterHeight > textHeight)
			{
				textHeight = letterHeight;
			}
		}
		TextRenderer::RenderText(renderer, x - textWidth / 2, y + textHeight / 2, text, font, color);
	}
}