#include "ColorPickerButton.h"
#include "IWindow.h"

ColorPickerButton::ColorPickerButton(IComponent* pParent, COLORREF color, int x, int y, int width, int height) :
	IHoverable(pParent),
	m_color(color)
{
	m_hWnd = CreateWindowExA(NULL, TEXT("Button"), "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, x, y, width, height, m_pParent->hWnd(), m_hMenu, NULL, NULL);
}

ColorPickerButton::~ColorPickerButton()
{
	DestroyWindow(m_hWnd);
}

bool ColorPickerButton::draw(LPDRAWITEMSTRUCT& pDIS)
{
	if (pDIS->hwndItem != m_hWnd)
		return false;

	HBRUSH brush = CreateSolidBrush(m_color);
	FillRect(pDIS->hDC, &pDIS->rcItem, IWindow::Resources::mainBkBrush);
	SelectObject(pDIS->hDC, brush);
	RoundRect(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom, 5, 5);

	if (m_hovering)
	{
		HPEN outline = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

		SelectObject(pDIS->hDC, outline);

		MoveToEx(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.top + 1, NULL);
		LineTo(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.bottom - 1);
		LineTo(pDIS->hDC, pDIS->rcItem.right - 1, pDIS->rcItem.bottom - 1);
		LineTo(pDIS->hDC, pDIS->rcItem.right - 1, pDIS->rcItem.top + 1);
		LineTo(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.top + 1);

		RECT tmp = { 0 };
		tmp.left = pDIS->rcItem.left, tmp.right = tmp.left + 1;
		tmp.top = pDIS->rcItem.bottom, tmp.bottom = tmp.top - 1;
		FillRect(pDIS->hDC, &tmp, IWindow::Resources::mainBkBrush);
		tmp.left = pDIS->rcItem.right, tmp.right = tmp.left - 1;
		tmp.top = pDIS->rcItem.bottom, tmp.bottom = tmp.top - 1;
		FillRect(pDIS->hDC, &tmp, IWindow::Resources::mainBkBrush);
	}

	return true;
}

void ColorPickerButton::click()
{
	COLORREF custom[16] = { 0 };
	CHOOSECOLOR cc = { 0 };
	cc.hwndOwner = m_pParent->hWnd();
	cc.lpCustColors = custom;
	cc.rgbResult = m_color;
	cc.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
	cc.lStructSize = sizeof(cc);
	ChooseColor(&cc);
	m_color = cc.rgbResult;
	InvalidateRgn(m_hWnd, nullptr, false);
}

COLORREF ColorPickerButton::getColor() const
{
	return m_color;
}