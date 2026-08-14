#pragma once

#include <string>

#include "IComponent.h"

/// Generic edit control component derived from IComponent.
/// Used for receiving text input from the user.
class Edit : public IComponent
{
public:
	/// Constructs an Edit component.
	/// 
	/// @param pParent - pointer to the parent component.
	/// @param text - text that will fill edit initially.
	/// @param x, y - coordinates, relative to parent window.
	/// @param width, height - size of the edit control in pixels.
	/// @param additionalStyles - WinAPI style flags that will be added to WS_CHILD, WS_VISIBLE and ES_AUTOHSCROLL flags. Can be ignored.
	Edit(IComponent* pParent, const std::string& text, int x, int y, int width, int height, DWORD additionalStyles = 0);
	Edit(IComponent* pParent, const std::wstring& text, int x, int y, int width, int height, DWORD additionalStyles = 0);

	~Edit();

	/// @return True if the edit control is empty, false otherwise.
	bool isEmpty() const noexcept { return textW().empty(); }
};
