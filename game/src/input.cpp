#include "input.h"

bool g_IsDown[256] = {};
bool g_IsDownFirstTime[256] = {};
bool g_MouseButtonState[8] = {};
ks::Vec2 g_MousePosition = {};

namespace ks::Input
{
	bool IsKeyDown(KeyCodes key)
	{
		return g_IsDown[(char)key];
	}

	bool IsKeyPressed(KeyCodes key)
	{
		return g_IsDownFirstTime[(char)key];
	}

	bool IsMouseKeyDown(MouseKeyCodes key)
	{
		return g_MouseButtonState[(char)key];
	}

	void Update()
	{
		for (int i = 0; i < 256; i++)
		{
			g_IsDownFirstTime[i] = false;
		}
	}

	Vec2 GetMousePosition()
	{
		return g_MousePosition;
	}
}