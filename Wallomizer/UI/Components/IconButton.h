#pragma once

#include "IHoverable.h"

/// Class of custom drawn "button" component. Derrives IHoverable interface.
/// Component usually used for recieving click events.
/// Needs to be dynamically allocated in response to WinAPI WM_CREATE message.
/// Needs to be destroyed in response to WM_DESTROY message.
/// Draw method should be called in response to WM_DRAWITEM message. 
/// Return TRUE from function that handles WinAPI messages if component is drawn.
/// To check for click event you should check result of isClicked method when WM_COMMAND message recieved.
/// After click event processed you should return 0 from function that handles WinAPI messages.
/// To process hovering events you should call mouseHovering method in response to WM_SETCURSOR message.
class IconButton : public IHoverable
{
public:
	/// Icon button component construction.
	/// 
	/// @param pParent - pointer to parent component
	/// @param x, y - coordinates, relative to parent window.
	/// @param width, height - size of button in pixels.
	/// @param hIcon - handle to icon that represents button in it's basic state.
	/// @param hIconHover - handle to icon that represents button in it's hovered state.
	/// @param additionalStyles - WinAPI style flags that will be added to WS_CHILD, WS_VISIBLE and BS_OWNERDRAW flags. Can be ignored.
	/// @param additionalExStyles - WinAPI extended style flags. Can be ignored.
	IconButton(IComponent* pParent, int x, int y, int width, int height, HICON& hIcon, HICON& hIconHovered, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	~IconButton();

	/// Call this method from reaction to WM_DRAWITEM message.
	/// 
	/// @param pDIS - LPDRAWITEMSTRUCT casted from lParam.
	/// @param bkgrnd - brush for background to draw on.
	/// 
	/// @return True if component is drawn, false otherwise. 
	///			If it's true return TRUE from function that handles WinAPI messages. 
	bool draw(LPDRAWITEMSTRUCT& pDIS, HBRUSH bkgrnd, int x = 0, int y = 0);

private:
	/// Handles to icons which represents button in basic and hovered state.
	HICON m_hIcon, m_hIconHovered;
};