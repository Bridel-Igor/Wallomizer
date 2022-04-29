#include "IHoverable.h"

void IHoverable::mouseHovering(WPARAM wParam)
{
	const bool isHovering = (HWND)wParam == m_hWnd;
	if (m_hovering != isHovering)
	{
		m_hovering = isHovering;
		if (m_hWnd)
			InvalidateRect(m_hWnd, NULL, FALSE);
	}
}