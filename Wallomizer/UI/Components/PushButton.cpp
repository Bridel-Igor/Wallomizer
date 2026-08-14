#include "PushButton.h"

#include "UIColor.h"

#pragma comment(lib, "Msimg32.lib")

PushButton::PushButton(IComponent* pParent, const std::string& text, int x, int y, int width, int height, DWORD additionalStyles, DWORD additionalExStyles, COLORREF checkedColorTop, COLORREF checkedColorBottom) :
	IHoverable(pParent),
	m_checkedColorTop(checkedColorTop),
	m_checkedColorBottom(checkedColorBottom)
{
	m_hWnd = CreateWindowExA(additionalExStyles, "Button", text.c_str(), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, parent()->hWnd(), hMenu(), nullptr, nullptr);
}

PushButton::~PushButton()
{
	DestroyWindow(m_hWnd);
}

void PushButton::check(bool state) noexcept
{
	if (m_checked != state)
	{
		m_checked = state;
		InvalidateRect(m_hWnd, nullptr, true);
	}
}

bool PushButton::draw(LPDRAWITEMSTRUCT pDIS) const
{
	if (pDIS->hwndItem != m_hWnd)
		return false;

	const BYTE hoverOffset = m_hovering ? 10 : 0;

	const BYTE topUnchecked = 70 + hoverOffset;
	const BYTE bottomUnchecked = 40 + hoverOffset;

	const COLORREF colorTop = m_checked ?
		m_checkedColorTop :
		RGB(topUnchecked, topUnchecked, topUnchecked);

	const COLORREF colorBottom = m_checked ?
		m_checkedColorBottom :
		RGB(bottomUnchecked, bottomUnchecked, bottomUnchecked);

	TRIVERTEX vertices[2]{
		{
			pDIS->rcItem.left,
			pDIS->rcItem.top,
			static_cast<COLOR16>(GetRValue(colorTop) << 8),
			static_cast<COLOR16>(GetGValue(colorTop) << 8),
			static_cast<COLOR16>(GetBValue(colorTop) << 8),
			0
		},
		{
			pDIS->rcItem.right,
			pDIS->rcItem.bottom,
			static_cast<COLOR16>(GetRValue(colorBottom) << 8),
			static_cast<COLOR16>(GetGValue(colorBottom) << 8),
			static_cast<COLOR16>(GetBValue(colorBottom) << 8),
			0
		}
	};

	GRADIENT_RECT gradient{ 0, 1 };
	GradientFill(pDIS->hDC, vertices, 2, &gradient, 1, GRADIENT_FILL_RECT_V);

	// draw text only if button is horizontal
	if ((pDIS->rcItem.right - pDIS->rcItem.left) > (pDIS->rcItem.bottom - pDIS->rcItem.top))
	{
		SetTextColor(pDIS->hDC, m_checked ? UIColor::pbTextActive : UIColor::pbTextInactive);
		SetBkMode(pDIS->hDC, TRANSPARENT);
		SetTextAlign(pDIS->hDC, TA_CENTER);
		const std::string text = textA();
		TextOutA(pDIS->hDC, (pDIS->rcItem.right - pDIS->rcItem.left) / 2, pDIS->rcItem.top + 3, text.c_str(), static_cast<int>(text.length()));
	}

	return true;
}
