#pragma once

#include "IHoverable.h"

/// Class of custom drawn "check box" component. Derrives IHoverable interface.
/// Component usually used for recieving logical (true or false) answers from user.
/// Needs to be dynamically allocated in response to WinAPI WM_CREATE message.
/// Needs to be destroyed in response to WM_DESTROY message.
/// Draw method should be called in response to WM_DRAWITEM message. 
/// Return TRUE from function that handles WinAPI messages if component is drawn.
/// To check for click event you should check result of isClicked method when WM_COMMAND message recieved.
/// After click event processed you should return 0 from function that handles WinAPI messages.
/// To process hovering events you should call mouseHovering method in response to WM_SETCURSOR message.
class CheckBox : public IHoverable
{
public:
	/// Check box component construction.
	/// 
	/// @param hParent - HWND of parent window.
	/// @param x, y - coordinates, relative to parent window.
	/// @param width, height - size of button in pixels.
	/// @param isChecked - initial state of component. Can be ignored.
	/// @param additionalStyles - WinAPI style flags that will be added to WS_CHILD, WS_VISIBLE and BS_OWNERDRAW flags. Can be ignored.
	/// @param additionalExStyles - WinAPI extended style flags. Can be ignored.
	CheckBox(HWND hParent, int x, int y, int width, int height, bool isChecked = false, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	~CheckBox();

	/// Flips state of check box. 
	/// Use in reaction to click event.
	void click();

	/// Changes state of check box.
	/// 
	/// @param state - desired state of check box.
	void setChecked(bool state);

	/// Check the state of check box.
	/// 
	/// @return True if check box is checked, false otherwise.
	bool isChecked();

	/// Call this method from reaction to WM_DRAWITEM message.
	/// 
	/// @param pDIS - LPDRAWITEMSTRUCT casted from lParam.
	/// @param bkgrnd - brush for background to draw on.
	/// 
	/// @return True if component is drawn, false otherwise. 
	///			If it's true return TRUE from function that handles WinAPI messages. 
	bool draw(LPDRAWITEMSTRUCT& pDIS, HBRUSH bkgrnd);
	// HACK: move draw methods to IDrawable interface

private:
	/// Holds the current state of check box. // HACK: move all checked methods to ICheckable interface
	bool m_checked;
};