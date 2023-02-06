#pragma once

#include "types.h"
#include "../color.h"
#include "../math/matrix4x4.h"

namespace ks
{
	struct Window;
	using VertexBufferHandle = uint32;
	using VertexArrayHandle = uint32;
	using ShaderHandle = uint32;
	using TextureHandle = uint32;
    
	enum class VertexBufferUsage
	{
		Static,
		Dynamic
	};
    
	enum RendererVersion
	{
		OPEN_GL_3_3,
		OPEN_GL_4_6
	};
    
	struct RenderState;
	struct Font;

    struct TextRenderer
    {
        static void RenderText(TextRenderer* renderer, float x, float y,
                               const char* text,
                               Font* font,
                               Color color = Color::BLACK);

		static void RenderTextCentered(TextRenderer* renderer, float x, float y,
									   const char* text,
									   Font* font,
									   Color color = Color::BLACK);

        VertexArrayHandle  VertexArray;
        VertexBufferHandle VertexBuffer;
        ShaderHandle       Shader;
		Matrix4x4*		   Projection;
    };
    
	namespace Renderer
	{
        
        // TODO(Fabian): Move all these create functions into their corresponding
        // namespaces
		VertexBufferHandle CreateVertexBuffer(RenderState& state, const float32* pData, size_t size, VertexBufferUsage usage = VertexBufferUsage::Static);
        
		ShaderHandle CreateShader(RenderState& state, const string vertexSource,
                                  const string fragmentSource, const string geometrySource = nullptr);
        
		VertexArrayHandle CreateVertexArray(RenderState& state);
        
		void ReleaseVertexBuffer(VertexBufferHandle handle, RenderState& state);
		void ReleaseShader(ShaderHandle handle, RenderState& state);
		void ReleaseVertexArray(VertexArrayHandle handle, RenderState& state);
        
	}
    
    
	bool IsRendererInitialized();
	RenderState* InitRenderer(Window& window, RendererVersion version);
	void SetViewportSize(uint32 width, uint32 height);
	void SetClearColor(float32 r, float32 g, float32 b, float32 a);
	void ClearBuffer();
	void DrawElements(uint32 count);
}