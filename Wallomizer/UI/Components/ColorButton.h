#pragma once

#include "IHoverable.h"

/// Class of custom drawn "color button" component. Derrives IHoverable interface.
/// Component usually used for recieving logical (true or false) answers from user about using colors.
/// Needs to be dynamically allocated in response to WinAPI WM_CREATE message.
/// Needs to be destroyed in response to WM_DESTROY message.
/// Draw method should be called in response to WM_DRAWITEM message. 
/// Return TRUE from function that handles WinAPI messages if component is drawn.
/// To check for click event you should check result of isClicked method when WM_COMMAND message recieved.
/// After click event processed you should return 0 from function that handles WinAPI messages.
/// To process hovering events you should call mouseHovering method in response to WM_SETCURSOR message.
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
		/// Handles to icons that holds bitmaps of check mark and appearence of empty color button.
		static HICON s_hICheckWhite, s_hICheckBlack, s_hIColorEmpty;
	private:
		static unsigned char s_refCount;
	}resources;
public:
	/// Color button component construction.
	/// 
	/// @param pParent - pointer to parent component
	/// @param red, green, blue - value of color chanels in range [0, 255].
	/// @param x, y - coordinates, relative to parent window.
	/// @param width, height - size of button in pixels.
	/// @param empty - flag if no color on button needs to be set. Can be ignored.
	ColorButton(IComponent *pParent, BYTE red, BYTE green, BYTE blue, int x, int y, int width, int height, bool empty = false);
	~ColorButton();

	/// Changes state of color button.
	/// 
	/// @param state - desired state of color button.
	void check(bool state);

	/// Check the state of color button.
	/// 
	/// @return True if color button is checked, false otherwise.
	bool isChecked() const;

	/// Call this method from reaction to WM_DRAWITEM message.
	/// 
	/// @param pDIS - LPDRAWITEMSTRUCT casted from lParam.
	/// 
	/// @return True if component is drawn, false otherwise. 
	///			If it's true return TRUE from function that handles WinAPI messages. 
	bool draw(LPDRAWITEMSTRUCT& pDIS);

	/// Get an ASCII representation of color in hex. You should provide char array to write it to.
	/// 
	/// @param buffer - pointer to char array
	/// @param size - size of the char array
	void getColor(wchar_t* buffer, int size);

private:
	/// Holds the current state of check box.
	bool m_checked = false;
	/// Holds flag if no color on button.
	bool m_empty = false;
	/// values of color chanels in range [0, 255].
	BYTE m_red, m_green, m_blue;
	/// Flags if checked color must be white or black. Used for drawing.
	bool m_checkedPenIsWhite;
	/// Handle for brush which holds the color of the button. Used for drawing.
	HBRUSH m_brush;
};