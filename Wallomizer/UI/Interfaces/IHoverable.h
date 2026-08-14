#pragma once

#include "IClickable.h"

class IWindow;

/// Interface for components that react to mouse hovering.
/// Provides hover state and invalidates the component when the state changes.
class IHoverable : public IClickable
{
public:
	/// Updates the mouse hovering state.
	/// Should be called in response to WM_SETCURSOR.
	///
	/// @param wParam - WPARAM received with WM_SETCURSOR.
	void handleMouseHover(WPARAM wParam) noexcept;

protected:
	/// Protected constructor to prevent direct instantiation.
	/// 
	/// @param pParent - pointer to parent component.
	explicit IHoverable(IComponent* pParent);

	~IHoverable();

	/// Indicates whether the mouse is currently over the component.
	bool m_hovering = false;

private:
	IWindow* m_pParentWindow = nullptr;
};
