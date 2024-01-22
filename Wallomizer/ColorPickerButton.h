#pragma once

#include <Windows.h>

#include "IHoverable.h"

/// Class of custom drawn "color button" component. Derrives IHoverable interface.
/// Component is used for getting user defined color.
/// Draw method should be called in response to WM_DRAWITEM message. 
/// Return TRUE from function that handles WinAPI messages if component is drawn.
/// To check for click event you should check result of isClicked method when WM_COMMAND message recieved.
/// After click event processed you should return 0 from function that handles WinAPI messages.
/// To process hovering events you should call mouseHovering method in response to WM_SETCURSOR message.
class ColorPickerButton : public IHoverable
{
public:
	/// Color picker button component construction.
	/// 
	/// @param pParent - pointer to parent component
	/// @param color - value of initial color. You can use RGB macro.
	/// @param x, y - coordinates, relative to parent window.
	/// @param width, height - size of button in pixels.
	ColorPickerButton(IComponent* pParent, COLORREF color, int x, int y, int width, int height);
	~ColorPickerButton();

	/// Call this method from reaction to WM_DRAWITEM message.
	/// 
	/// @param pDIS - LPDRAWITEMSTRUCT casted from lParam.
	/// 
	/// @return True if component is drawn, false otherwise. 
	///			If it's true return TRUE from function that handles WinAPI messages. 
	bool draw(LPDRAWITEMSTRUCT& pDIS);

	/// Opens ChooseColor window to change color that this button holds.
	/// Call this method when click is detected or you want to emulate click.
	void click();

	/// Get color
	///
	/// @return Current color value.
	COLORREF getColor() const;

private:
	/// Color value that this button holds. It can be set by constructor or click method.
	COLORREF m_color;
};