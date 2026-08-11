#pragma once

#include "IComponent.h"
#include "HMenuId.h"

/// Interface for components that receive click notifications from WinAPI.
/// Provides a unique WinAPI control identifier and click detection.
class IClickable : public IComponent
{
public:
	/// Detects whether the component was clicked.
	/// Should be called in response to WM_COMMAND.
	///
	/// @param wParam - WPARAM received with WM_COMMAND.
	/// @return True if the component was clicked.
	bool isClicked(WPARAM wParam) const noexcept
	{
		return LOWORD(wParam) == m_hMenu.value() 
			&& HIWORD(wParam) == BN_CLICKED;
	}

	/// @return WinAPI menu handle associated with the component.
	HMENU hMenu() const noexcept 
	{ 
		return reinterpret_cast<HMENU>(static_cast<UINT_PTR>(m_hMenu.value())); 
	}

protected:
	/// Protected constructor to prevent direct instantiation of an interface.
	/// 
	/// @param pParent - pointer to parent component.
	explicit IClickable(IComponent* pParent) :
		IComponent(pParent)
	{}

	~IClickable() = default;

private:
	/// Unique WinAPI control identifier.
	HMenuId m_hMenu;
};
