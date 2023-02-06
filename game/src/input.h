#pragma once

#include "platform/keycodes.h"
#include "math/vec2.h"

namespace ks::Input
{
	bool IsKeyDown(KeyCodes key);
	bool IsKeyPressed(KeyCodes key);
	bool IsMouseKeyDown(MouseKeyCodes key);
	void Update();
	Vec2 GetMousePosition();
}