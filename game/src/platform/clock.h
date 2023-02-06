#pragma once

#include "types.h"

namespace ks::Clock
{
	// NOTE: Returns the ellapsed time since application
	// startup in milliseconds
	int32 Now();
}