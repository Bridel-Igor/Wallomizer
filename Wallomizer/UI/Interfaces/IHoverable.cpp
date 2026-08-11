#include "IHoverable.h"

void IHoverable::handleMouseHover(WPARAM wParam) noexcept
{
	const bool isHovering = reinterpret_cast<HWND>(wParam) == m_hWnd;
	if (m_hovering == isHovering)
		return;

	m_hovering = isHovering;
	if (m_hWnd)
		InvalidateRect(m_hWnd, nullptr, FALSE);
}
