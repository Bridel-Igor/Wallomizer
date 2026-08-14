#pragma once

#include <string>

#include "IComponent.h"

/// Generic ComboBox component.
/// 
/// Selection changes are reported through WM_COMMAND with CBN_SELCHANGE.
/// Dropdown opening is reported through WM_COMMAND with CBN_DROPDOWN.
class ComboBox : public IComponent
{
public:
	/// Constructs a ComboBox component.
	/// 
	/// @param pParent - pointer to the parent component
	/// @param text - text displayed by the component.
	/// @param x, y - coordinates, relative to the parent window.
	/// @param width, height - size of the component in pixels.
	/// @param additionalStyles - WinAPI style flags that will be added to
	/// CBS_DROPDOWNLIST, CBS_HASSTRINGS, WS_CHILD, WS_VISIBLE and WS_VSCROLL flags. Can be ignored.
	ComboBox(IComponent* pParent, const std::wstring& text, int x, int y, int width, int height, DWORD additionalStyles = 0);

	~ComboBox();

	/// Adds an option to the combo box.
	void addOption(const std::wstring& text);

	/// Removes all options.
	void clear();

	/// Selects an option by index.
	void selectOption(int index) noexcept;

	/// Returns the index of the currently selected option,
	/// or CB_ERR if no option is selected.
	int getSelectedOption() const noexcept;

	/// Finds an option with exact text.
	/// Returns its index, or CB_ERR if not found.
	int findOption(const std::wstring& text) const noexcept;
};
