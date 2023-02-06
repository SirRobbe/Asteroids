#ifdef KS_WIN32
#include "../window.h"
#include "../renderer.h"
#include "kswindows.h"
#include <windowsx.h>
#include "../src/math/vec2.h"
#include "../../input.h"

#include "glad/glad_wgl.h"
// TODO: custom assert
#include <cassert>
#include <cstdlib>
#include <stdio.h>

extern bool g_IsDown[256];
extern bool g_IsDownFirstTime[256];
extern bool g_MouseButtonState[8];
extern ks::Vec2 g_MousePosition;
bool g_IsRunning = true;

namespace ks
{
	struct Window
	{
		HWND WindowHandle;
		HDC DeviceContext;
		HGLRC OpenGLContext;
	};

	INTERNAL LRESULT  CALLBACK Win32WindowCallback(HWND hwnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = 0;
		switch(uMsg)
		{
			case WM_CLOSE:
			{
				PostQuitMessage(0);
			}
			break;

			case WM_SIZE:
			{
				if(IsRendererInitialized())
				{
					RECT dim;
					if(GetClientRect(hwnd, &dim) == FALSE)
					{
						// TODO: Diagnostics ...
						assert(false);
					}
					SetViewportSize(dim.right, dim.bottom);
				}
				
			}
			break;

			case WM_KEYDOWN:
			{
				g_IsDown[wParam] = true;
				int previousState = (lParam >> 30) & 1;
				g_IsDownFirstTime[wParam] = previousState == 0 ? true : false;
			}
			break;

			case WM_KEYUP:
			{
				g_IsDown[wParam] = false;
				g_IsDownFirstTime[wParam] = false;
			}
			break;

			case WM_MOUSEMOVE:
			{
				g_MousePosition.X = GET_X_LPARAM(lParam);
				g_MousePosition.Y = GET_Y_LPARAM(lParam);
			}
			break;

			case WM_LBUTTONDOWN:
			{
				g_MouseButtonState[(char)ks::MouseKeyCodes::LeftButton] = true;
			}
			break;

			case WM_LBUTTONUP:
			{
				g_MouseButtonState[(char)ks::MouseKeyCodes::LeftButton] = false;
			}
			break;

			default:
			{
				result = DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
			break;
		}

		return result;
	}

	Window* ks::PlatformCreateWindow(uint32 width, uint32 height, const stringUTF8 title)
	{
		Window* pWindow = new Window();
		HINSTANCE instance = GetModuleHandle(nullptr);

		WNDCLASSEX windowClass = {};
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc = Win32WindowCallback;
		windowClass.hInstance = instance;
		windowClass.lpszClassName = (LPCWSTR)u"KeySpotterWindowClass";

		if(RegisterClassEx(&windowClass))
		{
			OutputDebugStringA("REGISTERED CLASS\n");
		}
		else
		{
			// TODO: Diagnostics
			assert(false);
		}

		RECT clientArea = {};
		clientArea.left = 100;
		clientArea.top = 100;
		clientArea.right = clientArea.left + width;
		clientArea.bottom = clientArea.top + height;

		if(!AdjustWindowRect(&clientArea, WS_OVERLAPPEDWINDOW, FALSE))
		{
			// TODO: Diagnostics ...
			assert(false);
		}

		LPWSTR titleUTF16 = Win32UTF8ToUTF16(title);

		if(!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE))
		{
			// TODO: Diagnostics ...
			assert(false);
		}

		pWindow->WindowHandle = CreateWindowEx(0, (LPCWSTR)u"KeySpotterWindowClass",
			titleUTF16, WS_OVERLAPPEDWINDOW, clientArea.left, clientArea.top,
			clientArea.right - clientArea.left, clientArea.bottom - clientArea.top,
			nullptr, nullptr, instance, nullptr);

		if(pWindow->WindowHandle)
		{
			OutputDebugStringA("CREATED WINDOW\n");
		}
		else
		{
			// TODO: Diagnostics
			assert(false);

		}

		ShowWindow(pWindow->WindowHandle, SW_SHOW);
		return pWindow;
	}

	void ks::PlatformPollEvents()
	{
		MSG msg = {};
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				g_IsRunning = false;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void ks::SwapBuffers(const Window& window)
	{
		if(!SwapBuffers(window.DeviceContext))
		{
			// TODO: Diagnostics ...
			assert(false);
		}
	}

	void CreateRenderContext(Window& window, RendererVersion version)
	{
		PIXELFORMATDESCRIPTOR pixelFormat = {};
		pixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pixelFormat.nVersion = 1;
		pixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
		pixelFormat.iPixelType = PFD_TYPE_RGBA;
		pixelFormat.cColorBits = 32;
		pixelFormat.cDepthBits = 24;
		pixelFormat.cStencilBits = 8;
		pixelFormat.iLayerType = PFD_MAIN_PLANE;

		window.DeviceContext = GetDC(window.WindowHandle);
		if(window.DeviceContext == nullptr)
		{
			// TODO: Diagnostics ...
			assert(false);
		}

		int32 bestPixelFormat = 0;
		bestPixelFormat = ChoosePixelFormat(window.DeviceContext, &pixelFormat);
		SetPixelFormat(window.DeviceContext, bestPixelFormat, &pixelFormat);
		HGLRC tempContext;
		tempContext = wglCreateContext(window.DeviceContext);
		wglMakeCurrent(window.DeviceContext, tempContext);

		int majorVersion = 4;
		int minorVersion = 6;
		if (version == RendererVersion::OPEN_GL_3_3)
		{
			majorVersion = 3;
			minorVersion = 3;
		}

		int wglVersion = gladLoadWGL(window.DeviceContext);
		int attributes[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, minorVersion,
			WGL_CONTEXT_FLAGS_ARB,
			WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0
		};

		window.OpenGLContext = wglCreateContextAttribsARB(window.DeviceContext, NULL, attributes);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(window.DeviceContext, window.OpenGLContext);
		wglSwapIntervalEXT(1);
	}

	void GetWindowSize(const Window& window, uint32& width, uint32& height)
	{
		RECT dim = {};
		if(!GetClientRect(window.WindowHandle, &dim))
		{
			// TODO: Diagnostics ...
			assert(false);
		}
		width = dim.right;
		height = dim.bottom;
	}
}
#endif
