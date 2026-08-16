#include "IWindow.h"

#include <algorithm>
#include <stdexcept>

#include "IHoverable.h"
#include "UIColor.h"
#include "resource.h"

HFONT IWindow::Resources::mainFont;
HFONT IWindow::Resources::titleFont;
HBRUSH IWindow::Resources::mainBkBrush;

std::uint16_t IWindow::Resources::refCount = 0;

IWindow::Resources::Resources() noexcept
{
	if (refCount++) // Loading resources only if this is the first created window
		return;
	mainFont = CreateFontA(15, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_DONTCARE, "Arial");
	titleFont = CreateFontA(15, 0, 0, 0, FW_SEMIBOLD, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_DONTCARE, "Arial");
	mainBkBrush = CreateSolidBrush(UIColor::windowBk);
}

IWindow::Resources::~Resources() noexcept
{
	if (--refCount) // Destroying resources only if this is the last window destroyed
		return;
	DeleteObject(mainBkBrush);
	DeleteObject(mainFont);
	DeleteObject(titleFont);
}

IWindow::IWindow(IWindow* pOwner, LPCSTR sWindowName, std::string className, DWORD dwStyle, DWORD dwExStyle,
		int x, int y, int nWidth, int nHeight) :
	IComponent(nullptr),
	m_pOwner(pOwner),
	m_name(std::move(className))
{
	WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandleA(nullptr);
	wc.lpszClassName = m_name.c_str();
	wc.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_APP));
	if (RegisterClassA(&wc) == 0)
		throw std::runtime_error("Window class registration failed.");

	RECT rc{};
	rc.left = x;
	rc.right = x + nWidth;
	rc.top = y;
	rc.bottom = y + nHeight;
	AdjustWindowRect(&rc, dwStyle, FALSE);

	m_hWnd = CreateWindowExA(dwExStyle, m_name.c_str(), sWindowName, dwStyle, rc.left, rc.top,
		rc.right - rc.left, rc.bottom - rc.top, m_pOwner ? m_pOwner->hWnd() : nullptr, 0, GetModuleHandle(nullptr), this);
	if (!m_hWnd)
	{
		UnregisterClassA(m_name.c_str(), GetModuleHandleA(nullptr));
		throw std::runtime_error("Window creation failed.");
	}
}

IWindow::~IWindow() noexcept
{
	if (m_hWnd)
	{
		ShowWindow(hWnd(), SW_HIDE);
		DestroyWindow(m_hWnd);
	}

	if (m_pOwner)
	{
		EnableWindow(m_pOwner->hWnd(), TRUE);
		SetForegroundWindow(m_pOwner->hWnd());
	}

	if (!m_name.empty())
		UnregisterClassA(m_name.c_str(), GetModuleHandleA(nullptr));
}

void IWindow::windowLoop()
{
	m_isReady = true;
	PostMessageA(m_hWnd, WM_INITIALIZE_WINDOW, 0, 0);

	MSG msg{};
	while (GetMessageA(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	m_isReady = false;

	if (static_cast<UINT>(msg.wParam) == QUIT_APPLICATION && m_pOwner)
		m_pOwner->requestQuit();
}

void IWindow::focus()
{
	if (!m_hWnd)
		return;

	ShowWindow(m_hWnd, SW_RESTORE);
	SetForegroundWindow(m_hWnd);
}

void IWindow::requestQuit()
{
	PostMessageA(hWnd(), WM_QUIT, QUIT_APPLICATION, 0);
}

void IWindow::centerWindow(HWND hParent) noexcept
{
	if (!hParent)
		return;

	RECT parentRect, rect;
	GetWindowRect(hParent, &parentRect);
	GetWindowRect(m_hWnd, &rect);
	SetWindowPos(m_hWnd, nullptr, parentRect.left + ((parentRect.right - parentRect.left) / 2) - ((rect.right - rect.left) / 2),
		parentRect.top + ((parentRect.bottom - parentRect.top) / 2) - ((rect.bottom - rect.top) / 2),
		0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

BOOL CALLBACK IWindow::SetChildFont(HWND hChild, LPARAM lParam)
{
	SendMessage(hChild, WM_SETFONT, static_cast<WPARAM>(lParam), TRUE);
	return TRUE;
}

void IWindow::registerHoverable(IHoverable* pHoverable)
{
	if (pHoverable)
		m_hoverables.push_back(pHoverable);
}

void IWindow::unregisterHoverable(IHoverable* pHoverable)
{
	const auto it = std::find(m_hoverables.begin(), m_hoverables.end(), pHoverable);

	if (it != m_hoverables.end())
		m_hoverables.erase(it);
}

LRESULT CALLBACK IWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IWindow* pThis = nullptr;
	if (uMsg == WM_NCCREATE)
	{
		const auto* createStruct = reinterpret_cast<const CREATESTRUCTA*>(lParam);
		pThis = static_cast<IWindow*>(createStruct->lpCreateParams);
		SetWindowLongPtrA(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		pThis->m_hWnd = hWnd;
	}
	else
		pThis = reinterpret_cast<IWindow*>(GetWindowLongPtrA(hWnd, GWLP_USERDATA));

	switch (uMsg)
	{
	case WM_INITIALIZE_WINDOW:
	{
		EnumChildWindows(pThis->hWnd(), SetChildFont, reinterpret_cast<LPARAM>(pThis->resources.mainFont));
		if (pThis->m_pOwner)
		{
			EnableWindow(pThis->m_pOwner->hWnd(), FALSE);
			pThis->centerWindow(pThis->m_pOwner->hWnd());
			ShowWindow(pThis->hWnd(), SW_SHOWNORMAL);
			SetForegroundWindow(pThis->hWnd());
		}
	}
	return 0;

	case WM_CREATE:
	return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
	return 0;

	case WM_NCDESTROY:
		SetWindowLongPtrA(hWnd, GWLP_USERDATA, 0);
	return 0;

	case WM_CLOSE:
		DestroyWindow(hWnd);
	return 0;
	}

	if (pThis && pThis->m_isReady)
	{
		if(uMsg == WM_SETCURSOR)
			for (IHoverable* hoverable : pThis->m_hoverables)
				hoverable->handleMouseHover(wParam);

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
		SetTextColor(hdc, UIColor::editText);
		SetBkColor(hdc, UIColor::editBk);
		SetDCBrushColor(hdc, UIColor::editBk);
		return reinterpret_cast<LRESULT>(GetStockObject(DC_BRUSH));
	}

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, UIColor::staticText);
		SetBkColor(hdcStatic, UIColor::windowBk);
		return reinterpret_cast<LRESULT>(Resources::mainBkBrush);
	}

	case WM_CTLCOLORBTN:
	return reinterpret_cast<LRESULT>(GetSysColorBrush(COLOR_WINDOW + 1));
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}
