#pragma once

#include "IClickable.h"

/// Class of generic "button" component. Derrives IClickable interface.
/// Component usually used for recieving click events.
/// Needs to be dynamically allocated in response to WinAPI WM_CREATE message.
/// Needs to be destroyed in response to WM_DESTROY message.
/// To check for click event you should check result of isClicked method when WM_COMMAND message recieved.
/// After click event processed you should return 0 from function that handles WinAPI messages.
/// Hovering events are processed automatically.
class Button : public IClickable
{
public:
	/// Button component construction.
	/// 
	/// @param pParent - pointer to parent component
	/// @param text - text that will be displayed on button.
	/// @param x, y - coordinates, relative to parent window.
	/// @param width, height - size of button in pixels.
	/// @param additionalStyles - WinAPI style flags that will be added to WS_CHILD and WS_VISIBLE flags. Can be ignored.
	/// @param additionalExStyles - WinAPI extended style flags. Can be ignored.
	Button(IComponent* pParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	~Button();
};