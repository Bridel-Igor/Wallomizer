#pragma once

#include <Windows.h>

/// Namespace includes functions for hMenu organising.
/// Has a limit of 1000 simultaneous hMenu values.
/// Ensures unique values of generated hMenus.
namespace HMenuGenerator
{
	/// Randomly generates hMenu value which can be used for WinAPI component events.
	/// When generated hMenu is no longer needed it needs to be released by releaseHMenu(hMenu).	
	/// 
	/// @return Randomly generated hMenu in range [1000, 1999].
	HMENU getNewHMenu();

	/// Releases generated earlier hMenu value.
	/// 
	/// @param hMenu - hMenu that needs to be released.
	void releaseHMenu(HMENU hMenu);
}