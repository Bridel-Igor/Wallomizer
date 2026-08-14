#pragma once

#include <string>

#include "IComponent.h"

/// Generic "static" component. Derived from IComponent.
/// Component usually used for displaying text.
class Static : public IComponent
{
public:
	/// Constructs a Static component.
	/// 
	/// @param pParent - pointer to the parent component
	/// @param text - text displayed by the component.
	/// @param x, y - coordinates, relative to the parent window.
	/// @param width, height - size of the Static in pixels.
	/// @param additionalStyles - WinAPI style flags that will be added to WS_CHILD, WS_VISIBLE and SS_CENTERIMAGE flags. Can be ignored.
	/// @param additionalExStyles - WinAPI extended style flags. Can be ignored.
	Static(IComponent* pParent, const std::string& text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	Static(IComponent* pParent, const std::wstring& text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);

	~Static();
};
