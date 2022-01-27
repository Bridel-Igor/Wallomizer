#pragma once

#include "IComponent.h"

/// Interface that derives IComponent interface.
/// Needs to be derived by copmonent classes that will recieve click messages from WinAPI.
/// Provides component with unique HMENU value and click detection method
class IClickable : public IComponent
{
public:
	/// Handle to menu value of the component that used by WinAPI messaging.
	HMENU hMenu() {	return m_hMenu; }

	/// Method that detects if component has been clicked.
	/// Should be called in reaction to WM_COMMAND message.
	/// Event reaction should be initiated if method returns true.
	///
	/// @param wParam - WPARAM value that was recieved with WM_COMMAND message
	/// 
	/// @return Returns true if click was detected.
	bool isClicked(WPARAM wParam);

protected:
	/// Protected constructor to prevent allocation of an interface object.
	IClickable();
	~IClickable();

	/// Handle to menu value of the component that used by WinAPI messaging.
	HMENU m_hMenu = NULL;
};