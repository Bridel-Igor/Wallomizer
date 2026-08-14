#include "IHoverable.h"

#include "IWindow.h"

IHoverable::IHoverable(IComponent* pParent) :
	IClickable(pParent)
{
	IComponent* component = this;

	while (component->parent())
		component = component->parent();

	m_pParentWindow = static_cast<IWindow*>(component);
	m_pParentWindow->registerHoverable(this);
}

IHoverable::~IHoverable()
{
	m_pParentWindow->unregisterHoverable(this);
}

void IHoverable::handleMouseHover(WPARAM wParam) noexcept
{
	const bool isHovering = reinterpret_cast<HWND>(wParam) == m_hWnd;
	if (m_hovering == isHovering)
		return;

	m_hovering = isHovering;
	InvalidateRect(m_hWnd, nullptr, FALSE);
}
