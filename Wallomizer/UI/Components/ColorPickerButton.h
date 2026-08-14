#pragma once

#include <Windows.h>

#include "IHoverable.h"

/// Custom-drawn color picker button.
/// Opens the Windows color picker dialog when clicked.
/// 
/// Click events are detected by calling isClicked in response to WM_COMMAND.
/// Hover state is updated by calling handleMouseHover in response to WM_SETCURSOR.
/// The component is drawn by calling draw in response to WM_DRAWITEM.
class ColorPickerButton : public IHoverable
{
public:
	/// Constructs a color picker button.
	/// 
	/// @param pParent - pointer to the parent component.
	/// @param color - initial color. You can use RGB macro.
	/// @param x, y - coordinates, relative to the parent window.
	/// @param width, height - size of the component in pixels.
	ColorPickerButton(IComponent* pParent, COLORREF color, int x, int y, int width, int height);

	~ColorPickerButton();

	/// Draws the color picker button.
	/// 
	/// @param pDIS - LPDRAWITEMSTRUCT casted from lParam.
	/// 
	/// @return True if this component was drawn, false otherwise.
	/// If true is returned, the WM_DRAWITEM handler must return TRUE.
	bool draw(LPDRAWITEMSTRUCT pDIS) const;

	/// Opens the Windows color picker dialog and updates the selected color.
	/// Call this when a click is detected or when a click should be emulated.
	void click();

	/// @return Currently selected color.
	COLORREF getColor() const noexcept { return m_color; }

private:
	/// Currently selected color.
	COLORREF m_color;

	/// Resources used to draw the button.
	HBRUSH m_brush = nullptr;
	HPEN m_outlinePen = nullptr;
};
