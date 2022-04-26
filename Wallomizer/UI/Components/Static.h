#pragma once

#include "IComponent.h"

/// Class of generic "static" component. Derrives IComponent interface.
/// Component usually used for showing text.
/// Needs to be dynamically allocated in response to WinAPI WM_CREATE message.
/// Needs to be destroyed in response to WM_DESTROY message.
class Static : public IComponent
{
public:
	/// Static component construction.
	/// 
	/// @param hParent - HWND of parent window.
	/// @param text - text that will be displayed.
	/// @param x, y - coordinates, relative to parent window.
	/// @param width, height - size of button in pixels.
	/// @param additionalStyles - WinAPI style flags that will be added to WS_CHILD, WS_VISIBLE and SS_CENTERIMAGE flags. Can be ignored.
	/// @param additionalExStyles - WinAPI extended style flags. Can be ignored.
	Static(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	Static(HWND hParent, LPCWSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	~Static();
};