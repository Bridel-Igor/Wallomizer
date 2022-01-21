#pragma once

#include <Windows.h>

/// Interface, that should be derived by any visual WinAPI component directly or by deriving another Interface that derives it.
/// Contains Handle to window. Handle can be used by numerous WinAPI functions.
/// Handle lifetime needs to be controlled by derived component class.
class IComponent
{
public:
	IComponent& operator=(const IComponent&) = delete;
	IComponent(const IComponent&) = delete;
	IComponent() = default;

	/// @return Handle to window
	HWND hWnd() { return m_hWnd; }

protected:
	/// Handle to window
	HWND m_hWnd = NULL;
};