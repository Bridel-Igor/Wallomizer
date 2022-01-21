#include "ColorButton.h"
#include "WindowStyles.h"

int ColorButton::height, ColorButton::width;

ColorButton::ColorButton(HWND hParent, BYTE _red, BYTE _green, BYTE _blue, int x, int y, int _width, int _height, bool _empty):
	empty(_empty), red(_red), green(_green), blue(_blue)
{
	height = _height;
	width = _width;
	m_hWnd = CreateWindowExA(NULL, TEXT("Button"), "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, x, y, width, height, hParent, m_hMenu, NULL, NULL);
	brush = CreateSolidBrush(RGB(red, green, blue));
	pen = CreatePen(PS_NULL, 0, 0);
	checkedPen = CreatePen(PS_SOLID, 2, red + green + blue > 255 ? RGB(0, 0, 0) : RGB(255, 255, 255));
}

ColorButton::~ColorButton()
{
	DeleteObject(pen);
	DeleteObject(checkedPen);
	DeleteObject(brush);
	DestroyWindow(m_hWnd);
}

void ColorButton::check(bool state)
{
	if (checked != state)
	{
		checked = state;
		InvalidateRect(m_hWnd, nullptr, true);
	}
}

bool ColorButton::isChecked()
{
	return checked;
}

void ColorButton::draw(LPDRAWITEMSTRUCT& pDIS)
{
	FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::mainBkBrush);
	SelectObject(pDIS->hDC, pen);
	SelectObject(pDIS->hDC, brush);
	RoundRect(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom, 5, 5);
	if (empty)
		DrawIconEx(pDIS->hDC, 0, 0, WindowStyles::hIColorEmpty, 0, 0, 0, NULL, DI_NORMAL);
	if ((m_hovering && !checked) || checked)
	{
		SelectObject(pDIS->hDC, checkedPen);

		MoveToEx(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.top + 1, NULL);
		LineTo(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.bottom - 1);
		LineTo(pDIS->hDC, pDIS->rcItem.right - 1, pDIS->rcItem.bottom - 1);
		LineTo(pDIS->hDC, pDIS->rcItem.right - 1, pDIS->rcItem.top + 1);
		LineTo(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.top + 1);

		RECT tmp;
		tmp.left = pDIS->rcItem.left, tmp.right = tmp.left + 1;
		tmp.top = pDIS->rcItem.bottom, tmp.bottom = tmp.top - 1;
		FillRect(pDIS->hDC, &tmp, WindowStyles::mainBkBrush);
		tmp.left = pDIS->rcItem.right, tmp.right = tmp.left - 1;
		tmp.top = pDIS->rcItem.bottom, tmp.bottom = tmp.top - 1;
		FillRect(pDIS->hDC, &tmp, WindowStyles::mainBkBrush);
	}
	if (checked)
		DrawIconEx(pDIS->hDC, (pDIS->rcItem.right - 20) / 2, (pDIS->rcItem.bottom - 20) / 2,
					red + green + blue > 255 ? WindowStyles::hICheckBlack : WindowStyles::hICheckWhite,
					0, 0, 0, NULL, DI_NORMAL);
}

void ColorButton::getColor(char* buffer, int size)
{
	char c[3] = { 0 };
	_itoa_s(red, c, 16);
	if (strlen(c) == 1)
	{
		c[1] = c[0];
		c[0] = '0';
		c[2] = '\0';
	}
	strcat_s(buffer, size, c);
	_itoa_s(green, c, 16);
	if (strlen(c) == 1)
	{
		c[1] = c[0];
		c[0] = '0';
		c[2] = '\0';
	}
	strcat_s(buffer, size, c);
	_itoa_s(blue, c, 16);
	if (strlen(c) == 1)
	{
		c[1] = c[0];
		c[0] = '0';
		c[2] = '\0';
	}
	strcat_s(buffer, size, c);
}