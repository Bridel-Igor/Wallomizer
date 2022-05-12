#pragma once

#include <stack>
#include <exception>

#include "IWindow.h"
#include "IHoverable.h"
#include "Edit.h"

COLORREF IWindow::Resources::mainFontColor;
COLORREF IWindow::Resources::titleFontColor;
HFONT IWindow::Resources::mainFont;
HFONT IWindow::Resources::titleFont;
HBRUSH IWindow::Resources::mainBkBrush;

unsigned char IWindow::Resources::refCount = 0;

IWindow::Resources::Resources()
{
	if (refCount++) // Loading icons only if this is the first player creating
		return;
	mainFont = CreateFontA(15, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_DONTCARE, "Arial");
	titleFont = CreateFontA(15, 0, 0, 0, FW_SEMIBOLD, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_DONTCARE, "Arial");
	mainFontColor = RGB(129, 193, 193);
	titleFontColor = RGB(220, 220, 220);
	mainBkBrush = CreateSolidBrush(RGB(26, 26, 26));
}

IWindow::Resources::~Resources()
{
	if (--refCount) // Destroying objects only if this is the last player destroying
		return;
	DeleteObject(mainBkBrush);
	DeleteObject(mainFont);
	DeleteObject(titleFont);
}

IWindow::IWindow(LPCSTR sWindowName, LPCSTR sClassName, DWORD dwStyle, DWORD dwExStyle,
		int x, int y, int nWidth, int nHeight, IComponent* pParent) :
	IComponent(pParent),
	m_sName(sClassName)
{
	// Checking if this window is already exists
	m_hWnd = FindWindowA(sClassName, nullptr);
	if (m_hWnd) 
	{ 
		// if it exists then highlight it and cease construction of new one
		SetForegroundWindow(m_hWnd);
		throw std::exception("Window is already opened!");
		// HACK: change this to custom exception, or handle this differently!!!
	}

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandleA(NULL);
	wc.lpszClassName = m_sName;
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APP));
	if (RegisterClassA(&wc) == 0)
		throw std::exception("Window creation failed.");

	RECT rc = { 0 };
	rc.left = x;
	rc.right = x + nWidth;
	rc.top = y;
	rc.bottom = y + nHeight;
	AdjustWindowRect(&rc, dwStyle, FALSE);

	m_hWnd = CreateWindowExA(
		dwExStyle, m_sName, sWindowName, dwStyle, rc.left, rc.top,
		rc.right - rc.left, rc.bottom - rc.top, m_pParent ? m_pParent->hWnd() : nullptr, 0, GetModuleHandle(NULL), this);
	if (m_hWnd == FALSE)
		throw std::exception("Window creation failed.");
}

IWindow::~IWindow()
{
	DestroyWindow(m_hWnd);
	UnregisterClassA(m_sName, GetModuleHandleA(NULL));
}

void IWindow::windowLoop()
{
	m_isReady = true;
	MSG msg = { };
	while (GetMessageA(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	m_isReady = false;
}

void IWindow::centerWindow(HWND hParent)
{
	RECT parentRect, rect;
	GetWindowRect(hParent, &parentRect);
	GetWindowRect(m_hWnd, &rect);
	SetWindowPos(m_hWnd, NULL, parentRect.left + ((parentRect.right - parentRect.left) / 2) - ((rect.right - rect.left) / 2),
		parentRect.top + ((parentRect.bottom - parentRect.top) / 2) - ((rect.bottom - rect.top) / 2),
		0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

BOOL CALLBACK IWindow::SetChildFont(HWND hChild, LPARAM lParam)
{
	SendMessage(hChild, WM_SETFONT, (WPARAM)(HFONT)lParam, TRUE);
	return TRUE;
}

LRESULT CALLBACK IWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IWindow* pThis = NULL;
	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (IWindow*)pCreate->lpCreateParams;
		SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
		pThis->m_hWnd = hWnd;
	}
	else
		pThis = (IWindow*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);

	switch (uMsg)
	{
	case WM_CREATE:
	return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
	return 0;

	case WM_CLOSE:
		DestroyWindow(hWnd);
	return 0;
	}

	if (pThis && pThis->m_isReady)
	{
		switch (uMsg)
		{
		case WM_SETCURSOR:
		{
			pThis->traverseChildren([&wParam](IComponent* pComponent) 
			{
				IHoverable* pHoverable = nullptr;
				if ((pHoverable = dynamic_cast<IHoverable*>(pComponent)) != nullptr)
					pHoverable->mouseHovering(wParam);
			});
			// Fallthrough. Don't return as defWindowProc must be executed anyway.
		}
		}

		LRESULT result = pThis->HandleMessage(hWnd, uMsg, wParam, lParam);
		if (result != RESULT_DEFAULT)
			return result;
	}

	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, Resources::mainBkBrush);
		EndPaint(hWnd, &ps);
	}
	return 0;

	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, Edit::fontColor);
		SetBkColor(hdc, Edit::bkColor);
		SetDCBrushColor(hdc, Edit::bkColor);
		return (LRESULT)GetStockObject(DC_BRUSH);
	}

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, Resources::mainFontColor);
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)Resources::mainBkBrush;
	}

	case WM_CTLCOLORBTN:
	return (LRESULT)GetSysColorBrush(COLOR_WINDOW + 1);
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}