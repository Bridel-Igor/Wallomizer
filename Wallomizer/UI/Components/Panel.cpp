#include "Panel.h"

#include <utility>
#include <stdexcept>

#include "IWindow.h"
#include "IHoverable.h"

Panel::Panel(IComponent* pParent, std::string className, int x, int y, int width, int height, HBRUSH bkBrush) :
	IComponent(pParent),
	m_className(std::move(className)),
	m_bkBrush(bkBrush)
{
	WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandleA(nullptr);
	wc.lpszClassName = m_className.c_str();
	if (RegisterClassA(&wc) == 0)
		throw std::runtime_error("Panel creation failed.");

	RECT rc{};
	rc.left = x;
	rc.right = x + width;
	rc.top = y;
	rc.bottom = y + height;

	const DWORD STYLE = WS_CHILD | WS_BORDER | WS_VSCROLL | WS_VISIBLE;
	AdjustWindowRect(&rc, STYLE, FALSE);

	m_hWnd = CreateWindowExA(
		0, m_className.c_str(), "", STYLE, rc.left, rc.top,
		rc.right - rc.left, rc.bottom - rc.top, parent()->hWnd(), 0, GetModuleHandle(nullptr), this);
	if (!m_hWnd)
	{
		UnregisterClassA(m_className.c_str(), GetModuleHandleA(nullptr));
		throw std::runtime_error("Panel creation failed.");
	}
}

Panel::~Panel()
{
	DestroyWindow(m_hWnd);
	UnregisterClassA(m_className.c_str(), GetModuleHandleA(nullptr));
}

LRESULT CALLBACK Panel::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Panel* pThis = nullptr;
	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCTA* pCreate = reinterpret_cast<CREATESTRUCTA*>(lParam);
		pThis = static_cast<Panel*>(pCreate->lpCreateParams);
		SetWindowLongPtrA(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else
		pThis = reinterpret_cast<Panel*>(GetWindowLongPtrA(hWnd, GWLP_USERDATA));

	if (pThis)
		return pThis->HandleMessage(hWnd, uMsg, wParam, lParam);
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

LRESULT Panel::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, m_bkBrush);
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_COMMAND:
	case WM_SETCURSOR:
	case WM_DRAWITEM:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORSTATIC:
	case WM_VSCROLL:
	case WM_MOUSEWHEEL:
	{
		const LRESULT res = SendMessageA(parent()->hWnd(), uMsg, wParam, lParam);
		if (res != RESULT_NOT_HANDLED)
			return res;
		break;
	}
	}
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}
