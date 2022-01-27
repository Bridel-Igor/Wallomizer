#pragma once

#include "IHoverable.h"

/// Class of custom drawn "push button" component. Derrives IHoverable interface.
/// Component usually used for recieving logical (true or false) answers from user.
/// Needs to be dynamically allocated in response to WinAPI WM_CREATE message.
/// Needs to be destroyed in response to WM_DESTROY message.
/// Draw method should be called in response to WM_DRAWITEM message. 
/// Return TRUE from function that handles WinAPI messages if component is drawn.
/// To check for click event you should check result of isClicked method when WM_COMMAND message recieved.
/// After click event processed you should return 0 from function that handles WinAPI messages.
/// To process hovering events you should call mouseHovering method in response to WM_SETCURSOR message.
class PushButton : public IHoverable
{
public:
	/// Push button component construction.
	/// 
	/// @param hParent - HWND of parent window.
	/// @param text - text that will be displayed.
	/// @param x, y - coordinates, relative to parent window.
	/// @param width, height - size of button in pixels.
	/// @param additionalStyles - WinAPI style flags that will be added to WS_CHILD, WS_VISIBLE and BS_OWNERDRAW flags. Can be ignored.
	/// @param additionalExStyles - WinAPI extended style flags. Can be ignored.
	/// @param checkedColorTop, checkedColorBottom - color of top and bottom sides of the button in checked state. Draw methods do gradient between these colors. Can be ignored.
	PushButton(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0, COLORREF checkedColorTop = RGB(130, 130, 130), COLORREF checkedColorBottom = RGB(80, 80, 80));
	~PushButton();

	/// Changes state of push button.
	/// 
	/// @param state - desired state of push button.
	void check(bool state);

	/// Check the state of push button.
	/// 
	/// @return True if push button is checked, false otherwise.
	bool isChecked();

	/// Call this method from reaction to WM_DRAWITEM message.
	/// 
	/// @param pDIS - LPDRAWITEMSTRUCT casted from lParam.
	/// @param bkgrnd - brush for background to draw on.
	/// 
	/// @return True if component is drawn, false otherwise. 
	///			If it's true return TRUE from function that handles WinAPI messages. 
	void draw(LPDRAWITEMSTRUCT& pDIS);

private:
	/// Holds the current state of push button.
	bool m_checked = false;
	/// Color of top and bottom sides of the button in checked state. Draw methods do gradient between these colors.
	COLORREF m_checkedColorTop, m_checkedColorBottom;
	/// Temporary rectangle for some calculations in draw method.
	RECT m_temp = {0};
	/// Handle to brush that changes during gradient draw.
	HBRUSH m_color = NULL;
};