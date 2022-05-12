#include <exception>

#include "Panel.h"
#include "IWindow.h"
#include "IHoverable.h"

Panel::Panel(IComponent* pParent, LPCSTR className, int x, int y, int width, int height, HBRUSH bkBrush) :
	IComponent(pParent),
	m_sName(className),
	m_bkBrush(bkBrush)
{
	m_hWnd = FindWindowA(m_sName, nullptr);
	if (m_hWnd)
	{
		throw std::exception("Panel is already opened!");
		// HACK: change this to custom exception, or handle this differently!!!
	}

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandleA(NULL);
	wc.lpszClassName = m_sName;
	if (RegisterClassA(&wc) == 0)
		throw std::exception("Panel creation failed.");

	RECT rc = { 0 };
	rc.left = x;
	rc.right = x + width;
	rc.top = y;
	rc.bottom = y + height;
	AdjustWindowRect(&rc, WS_CHILD | WS_BORDER | WS_VSCROLL, FALSE);

	m_hWnd = CreateWindowExA(
		NULL, m_sName, "", WS_CHILD | WS_BORDER | WS_VSCROLL, rc.left, rc.top,
		rc.right - rc.left, rc.bottom - rc.top, m_pParent->hWnd(), 0, GetModuleHandle(NULL), this);
	if (m_hWnd == FALSE)
		throw std::exception("Panel creation failed.");
}

Panel::~Panel()
{
	DestroyWindow(m_hWnd);
	UnregisterClassA(m_sName, GetModuleHandleA(NULL));
}

LRESULT CALLBACK Panel::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Panel* pThis = NULL;
	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (Panel*)pCreate->lpCreateParams;
		SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
		pThis->m_hWnd = hWnd;
	}
	else
		pThis = (Panel*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);

	if (pThis)
		return pThis->HandleMessage(hWnd, uMsg, wParam, lParam);
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

LRESULT Panel::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		ShowWindow(hWnd, SW_SHOWNORMAL);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, m_bkBrush);
		EndPaint(hWnd, &ps);
		return 0;
	}	
	case WM_COMMAND:
	case WM_DRAWITEM:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORSTATIC:
	case WM_VSCROLL:
	case WM_MOUSEWHEEL:
		LRESULT res = SendMessageA(m_pParent->hWnd(), uMsg, wParam, lParam);
		if (res != RESULT_DEFAULT)
			return res;
	}
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}