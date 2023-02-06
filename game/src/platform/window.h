#pragma once

#include "types.h"
#include "renderer.h"

namespace ks
{
	struct Window;

	Window* PlatformCreateWindow(uint32 width, uint32 height, const stringUTF8 title);
	void PlatformPollEvents();
	void SwapBuffers(const Window& window);
	void CreateRenderContext(Window& window, RendererVersion version);
	void GetWindowSize(const Window& window, uint32& width, uint32& height);
}