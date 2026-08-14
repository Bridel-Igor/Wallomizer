#pragma once

#include <string>

#include "IClickable.h"

/// Generic button component.
/// Derives from IClickable and provides click event detection.
/// 
/// Clicks are detected using isClicked in response to WM_COMMAND.
/// Hovering events are processed automatically.
class Button : public IClickable
{
public:
	/// Constructs a button component.
	/// 
	/// @param pParent - pointer to the parent component
	/// @param text - text displayed by the component.
	/// @param x, y - coordinates, relative to the parent window.
	/// @param width, height - size of the component in pixels.
	/// @param additionalStyles - WinAPI style flags that will be added to WS_CHILD and WS_VISIBLE flags. Can be ignored.
	/// @param additionalExStyles - WinAPI extended style flags. Can be ignored.
	Button(IComponent* pParent, const std::string& text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	
	~Button() noexcept;
};
