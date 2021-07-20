#pragma once

#include "Windows.h"
#include <vector>

namespace HMenuGenerator
{
	HMENU getNewHMenu();
	void releaseHMenu(HMENU hMenu);
}