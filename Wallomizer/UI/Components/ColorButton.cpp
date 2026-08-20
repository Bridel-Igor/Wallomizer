#include "ColorButton.h"

#include "resource.h"
#include "IWindow.h"
#include "UIColor.h"
#include "GraphicsUtils.h"

ColorButton::Resources::Resources()
{
	penCheckedWhite = CreatePen(PS_SOLID, 2, UIColor::white);
	penCheckedBlack = CreatePen(PS_SOLID, 2, UIColor::black);
	penNull = CreatePen(PS_NULL, 0, 0);
	iconCheckWhite = GraphicsUtils::loadIcon(IDI_CHECK_WHITE);
	iconCheckBlack = GraphicsUtils::loadIcon(IDI_CHECK_BLACK);
	iconEmptyColor = GraphicsUtils::loadIcon(IDI_COLOR_EMPTY);
}

ColorButton::Resources::~Resources()
{
	DeleteObject(penCheckedWhite);
	DeleteObject(penCheckedBlack);
	DeleteObject(penNull);
	DestroyIcon(iconCheckWhite);
	DestroyIcon(iconCheckBlack);
	DestroyIcon(iconEmptyColor);
}

ColorButton::ColorButton(IComponent* pParent, BYTE red, BYTE green, BYTE blue, int x, int y, int width, int height, bool empty) :
	IHoverable(pParent),
	m_empty(empty), m_red(red), m_green(green), m_blue(blue),
	m_useWhiteCheck(m_red + m_green + m_blue < 255)
{
	m_hWnd = CreateWindowExA(0, "Button", "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, x, y, width, height, parent()->hWnd(), hMenu(), nullptr, nullptr);
	m_brush = CreateSolidBrush(RGB(m_red, m_green, m_blue));
}

ColorButton::~ColorButton()
{
	DeleteObject(m_brush);
	DestroyWindow(m_hWnd);
}

void ColorButton::setChecked(bool state)
{
	if (m_checked != state)
	{
		m_checked = state;
		InvalidateRect(m_hWnd, nullptr, FALSE);
	}
}

bool ColorButton::draw(LPDRAWITEMSTRUCT drawItem)
{
	if (drawItem->hwndItem != m_hWnd)
		return false;

	const Resources& resources = m_resources.get();

	FillRect(drawItem->hDC, &drawItem->rcItem, IWindow::Resources::mainBkBrush);
	SelectObject(drawItem->hDC, resources.penNull);
	SelectObject(drawItem->hDC, m_brush);
	RoundRect(drawItem->hDC, drawItem->rcItem.left, drawItem->rcItem.top, drawItem->rcItem.right, drawItem->rcItem.bottom, 5, 5);
	if (m_empty)
		DrawIconEx(drawItem->hDC, 0, 0, resources.iconEmptyColor, 0, 0, 0, nullptr, DI_NORMAL);
	if ((m_hovering && !m_checked) || m_checked)
	{
		SelectObject(drawItem->hDC, m_useWhiteCheck ? resources.penCheckedWhite : resources.penCheckedBlack);

		MoveToEx(drawItem->hDC, drawItem->rcItem.left + 1, drawItem->rcItem.top + 1, nullptr);
		LineTo(drawItem->hDC, drawItem->rcItem.left + 1, drawItem->rcItem.bottom - 1);
		LineTo(drawItem->hDC, drawItem->rcItem.right - 1, drawItem->rcItem.bottom - 1);
		LineTo(drawItem->hDC, drawItem->rcItem.right - 1, drawItem->rcItem.top + 1);
		LineTo(drawItem->hDC, drawItem->rcItem.left + 1, drawItem->rcItem.top + 1);

		RECT tmp = { 0 };
		tmp.left = drawItem->rcItem.left, tmp.right = tmp.left + 1;
		tmp.top = drawItem->rcItem.bottom, tmp.bottom = tmp.top - 1;
		FillRect(drawItem->hDC, &tmp, IWindow::Resources::mainBkBrush);
		tmp.left = drawItem->rcItem.right, tmp.right = tmp.left - 1;
		tmp.top = drawItem->rcItem.bottom, tmp.bottom = tmp.top - 1;
		FillRect(drawItem->hDC, &tmp, IWindow::Resources::mainBkBrush);
	}
	if (m_checked)
		DrawIconEx(drawItem->hDC, (drawItem->rcItem.right - 20) / 2, (drawItem->rcItem.bottom - 20) / 2,
					m_useWhiteCheck ? resources.iconCheckWhite : resources.iconCheckBlack,
					0, 0, 0, nullptr, DI_NORMAL);

	return true;
}

std::wstring ColorButton::getColor() const
{
	wchar_t buffer[7];
	swprintf_s(buffer, L"%02x%02x%02x", m_red, m_green, m_blue);
	return buffer;
}
