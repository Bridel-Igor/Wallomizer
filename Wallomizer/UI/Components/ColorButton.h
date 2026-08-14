#pragma once

#include <cstdint>
#include <string>

#include "IHoverable.h"

/// Custom-drawn color button component.
/// Derives from IHoverable and represents a selectable color.
///
/// Click events are detected by calling isClicked in response to WM_COMMAND.
/// Hover state is updated by calling handleMouseHover in response to WM_SETCURSOR.
/// The component is drawn by calling draw in response to WM_DRAWITEM.
class ColorButton : public IHoverable
{
private:
	/// Class manages resources (de)allocation for all ColorButton resources.
	class Resources
	{
	public:
		Resources();
		~Resources();

		/// Handles to pens to draw outlines.
		static HPEN s_checkedPenWhite, s_checkedPenBlack, s_nullPen;

		/// Icons used to draw the check mark and empty-color state.
		static HICON s_hICheckWhite, s_hICheckBlack, s_hIColorEmpty;

	private:
		static std::uint16_t s_refCount;
	}resources;

public:
	/// Constructs a ColorButton component.
	/// 
	/// @param pParent - pointer to the parent component.
	/// @param red, green, blue - value of color chanels in range [0, 255].
	/// @param x, y - coordinates, relative to the parent window.
	/// @param width, height - size of the component in pixels.
	/// @param empty - flag if no color on button needs to be set. Can be ignored.
	ColorButton(IComponent *pParent, BYTE red, BYTE green, BYTE blue, int x, int y, int width, int height, bool empty = false);

	~ColorButton();

	/// Changes state of color button.
	/// 
	/// @param state - desired state of color button.
	void setChecked(bool state);

	/// Check the state of color button.
	/// 
	/// @return True if color button is checked, false otherwise.
	bool isChecked() const noexcept { return m_checked; }

	/// Draws the color button.
	/// Should be called in response to WM_DRAWITEM.
	///
	/// @param pDIS - DRAWITEMSTRUCT received with WM_DRAWITEM.
	///
	/// @return True if this component was drawn, false otherwise.
	/// If true is returned, the WM_DRAWITEM handler must return TRUE.
	bool draw(LPDRAWITEMSTRUCT pDIS);

	/// @return Color represented as a lowercase hexadecimal RGB string.
	std::wstring getColor() const;

private:
	/// Indicates whether the button is checked.
	bool m_checked = false;

	/// Indicates whether the button represents an empty color.
	const bool m_empty;

	/// RGB color channels, each in the range [0, 255].
	const BYTE m_red, m_green, m_blue;

	/// Indicates whether the check mark should be drawn in white.
	const bool m_checkedPenIsWhite;

	/// Brush used to draw the button's selected color.
	HBRUSH m_brush;
};
