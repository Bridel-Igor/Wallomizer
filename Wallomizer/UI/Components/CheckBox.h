#pragma once

#include <stdint.h>

#include "IHoverable.h"

/// Custom-drawn check box component.
/// Derives from IHoverable and maintains its checked state.
/// 
/// Click events are detected by calling isClicked in response to WM_COMMAND.
/// Hover state is updated by calling handleMouseHover in response to WM_SETCURSOR.
/// The component is drawn by calling draw in response to WM_DRAWITEM.
class CheckBox : public IHoverable
{
private:
	/// Class manages resources (de)allocation for all CheckBox resources.
	class Resources
	{
	public:
		Resources();
		~Resources();

		static HICON hICheckBox, hICheckBoxChecked, hICheckBoxHover, hICheckBoxCheckedHover;
	private:
		static std::uint16_t refCount;
	}resources;
public:
	/// Constructs a CheckBox component.
	/// 
	/// @param pParent - pointer to the parent component.
	/// @param x, y - coordinates, relative to the parent window.
	/// @param width, height - size of the component in pixels.
	/// @param isChecked - initial state of the component. Can be ignored.
	/// @param additionalStyles - WinAPI style flags that will be added to WS_CHILD, WS_VISIBLE and BS_OWNERDRAW flags. Can be ignored.
	/// @param additionalExStyles - WinAPI extended style flags. Can be ignored.
	CheckBox(IComponent* pParent, int x, int y, int width, int height, bool isChecked = false, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	~CheckBox();

	/// Toggles the checked state of the check box.
	/// Use in response to a click event.
	void toggle() noexcept { setChecked(!m_checked); };

	/// Sets the checked state of the check box.
	/// 
	/// @param state - desired checked state.
	void setChecked(bool state) noexcept;

	/// @return True if check box is checked, false otherwise.
	bool isChecked() const noexcept { return m_checked; }

	/// Draws the check box.
	///
	/// Should be called in response to WM_DRAWITEM.
	/// 
	/// @param pDIS - LPDRAWITEMSTRUCT casted from lParam.
	/// @param bkgrnd - brush used to fill the background.
	/// 
	/// @return True if this component was drawn, false otherwise.
	/// If true is returned, the WM_DRAWITEM handler must return TRUE.
	bool draw(LPDRAWITEMSTRUCT pDIS, HBRUSH bkgrnd);

private:
	/// Holds the current state of check box.
	bool m_checked;
};
