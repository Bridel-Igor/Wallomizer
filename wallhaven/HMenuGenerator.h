#pragma once

#include <Windows.h>
#include <vector>

#define COMMANDEVENT(a) (a != nullptr && (HMENU)wParam == a->hMenu)

namespace HMenuGenerator
{
	HMENU getNewHMenu();
	void releaseHMenu(HMENU hMenu);
}