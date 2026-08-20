#pragma once

#include <string_view>

#include "IHoverable.h"

/// Class of custom drawn "button" component. Derrives IHoverable interface.
/// Component usually used for recieving click events.
/// 
/// Click events are detected by calling isClicked in response to WM_COMMAND.
/// Hover state is updated by calling handleMouseHover in response to WM_SETCURSOR.
/// The component is drawn by calling draw in response to WM_DRAWITEM.
class IconButton : public IHoverable
{
public:
	/// Constructs an IconButton component.
	/// 
	/// @param pParent - pointer to parent component.
	/// @param x, y - coordinates, relative to parent window.
	/// @param width, height - size of button in pixels.
	/// @param hIcon - handle to icon that represents button in it's basic state.
	/// @param hIconHover - handle to icon that represents button in it's hovered state.
	/// @param toolTip - optional tooltip text. The button does not own this string.
    ///                  The referenced string must outlive the button.
	/// @param additionalStyles - WinAPI style flags that will be added to WS_CHILD, WS_VISIBLE and BS_OWNERDRAW flags. Can be ignored.
	/// @param additionalExStyles - WinAPI extended style flags. Can be ignored.
	IconButton(IComponent* pParent, int x, int y, int width, int height, 
		HICON hIcon, HICON hIconHovered, HICON hIconDisabled = nullptr, HICON hIconToggled = nullptr, 
		std::string_view toolTip = {}, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	
	~IconButton();

	/// Draws the button.
	/// Call this method from reaction to WM_DRAWITEM message.
	/// 
	/// @param pDIS - LPDRAWITEMSTRUCT casted from lParam.
	/// @param bkgrnd - brush for background to draw on.
	/// 
	/// @return True if component is drawn, false otherwise. 
	///			If it's true return TRUE from function that handles WinAPI messages. 
	bool draw(LPDRAWITEMSTRUCT pDIS, bool enabled = true, bool toggled = false, HBRUSH bkgrnd = nullptr, int x = 0, int y = 0) const;

private:
	/// Icon displayed in the normal, hovered, disabled and toggled states. The button does not own those handles.
	HICON m_hIcon, m_hIconHovered, m_hIconDisabled, m_hIconToggled;

	/// Handle to looltip window.
	HWND m_hTooltip = nullptr;

	/// Non-owning view of the tooltip text.
	std::string_view m_tooltip;
};
