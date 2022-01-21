#pragma once

#include "IClickable.h"

/// Interface that derrives IClickable interface.
/// Needs to be derived by copmonent classes that will recieve mouse hovering and click messages from WinAPI.
/// Provides methods that react to mouseHovering messages.
class IHoverable : public IClickable
{
public:
	/// Method that reacts to mouse hovering notifications.
	/// Should be called in reaction to WM_SETCURSOR message.
	/// If mouse hovering status has changed initiates redraw of component.
	/// 
	/// @param wParam - WPARAM value that was recieved with WM_SETCURSOR message
	void mouseHovering(WPARAM wParam);

protected:
	/// Indicates mouse hovering status.
	/// Changed by the mouseHovering method.
	/// Used by draw method of components.
	bool m_hovering = false;
};