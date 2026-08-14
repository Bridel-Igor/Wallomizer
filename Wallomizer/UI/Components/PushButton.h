#pragma once

#include <string>

#include "IHoverable.h"
#include "UIColor.h"

/// Class of custom drawn "push button" component. Derives from IHoverable.
/// Component usually used for recieving logical (true or false) answers from user.
/// 
/// Click events are detected by calling isClicked in response to WM_COMMAND.
/// Hover state is updated by calling handleMouseHover in response to WM_SETCURSOR.
/// The component is drawn by calling draw in response to WM_DRAWITEM.
class PushButton : public IHoverable
{
public:
	/// Constructs a PushButton component.
	/// 
	/// @param pParent - pointer to the parent component
	/// @param text - text displayed by the component.
	/// @param x, y - coordinates, relative to the parent window.
	/// @param width, height - size of the component in pixels.
	/// @param additionalStyles - WinAPI style flags that will be added to WS_CHILD, WS_VISIBLE and BS_OWNERDRAW flags. Can be ignored.
	/// @param additionalExStyles - WinAPI extended style flags. Can be ignored.
	/// @param checkedColorTop - top gradient color when the button is checked
	/// @param checkedColorBottom - bottom gradient color when the button is checked
	PushButton(IComponent* pParent, const std::string& text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0, COLORREF checkedColorTop = UIColor::pbTopActive, COLORREF checkedColorBottom = UIColor::pbBottomActive);
	
	~PushButton();

	/// Sets the checked state of the button.
	/// 
	/// @param state - desired checked state.
	void check(bool state) noexcept;

	/// Check the state of push button.
	/// 
	/// @return True if push button is checked, false otherwise.
	bool isChecked() const noexcept { return m_checked; }

	/// Draws the button in response to WM_DRAWITEM.
	/// 
	/// @param pDIS - LPDRAWITEMSTRUCT casted from lParam.
	/// 
	/// @return True if component is drawn, false otherwise. 
	///			If true is returned, the WM_DRAWITEM message handler should return TRUE.
	bool draw(LPDRAWITEMSTRUCT pDIS) const;

private:
	/// Holds the current state of push button.
	bool m_checked = false;

	/// Top and bottom gradient colors used when the button is checked.
	const COLORREF m_checkedColorTop;
	const COLORREF m_checkedColorBottom;
};
