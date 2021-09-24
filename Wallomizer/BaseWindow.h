#pragma once

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>

#include "WindowStyles.h"
#include "resource.h"

template <class DERIVED_TYPE>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE* pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hWnd = hWnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		}
		if (pThis)
		{
			if (strcmp(pThis->ClassName(), "CollectionItemsFrameClass"))
			{
				switch (uMsg)
				{
				case WM_CTLCOLORSTATIC:
				{
					HWND hWndStatic = (HWND)lParam;
					HDC hdcStatic = (HDC)wParam;
					SetTextColor(hdcStatic, WindowStyles::mainFontColor);
					SetBkMode(hdcStatic, TRANSPARENT);
					return (LRESULT)WindowStyles::mainBkBrush;
				}
				return 0;

				case WM_CTLCOLOREDIT:
				{
					HDC hdc = (HDC)wParam;
					SetTextColor(hdc, RGB(0, 0, 0));
					SetBkColor(hdc, RGB(200, 200, 200));
					SetDCBrushColor(hdc, RGB(200, 200, 200));
					return (LRESULT)GetStockObject(DC_BRUSH);
				}
				return 0;

				case WM_CTLCOLORBTN:
				{
					return (LRESULT)GetSysColorBrush(COLOR_WINDOW + 1);
				}
				return 0;
				}
			}
			return pThis->HandleMessage(hWnd, uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	BaseWindow() : m_hWnd(NULL) { }

	BOOL Create(
		LPCSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
		)
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandleA(NULL);
		wc.lpszClassName = ClassName();
		wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));

		RegisterClass(&wc);
		
		RECT rc;
		rc.left = x;
		rc.right = x + nWidth;
		rc.top = y;
		rc.bottom = y + nHeight;
		AdjustWindowRect(&rc, dwStyle, FALSE);

		m_hWnd = CreateWindowExA(
			dwExStyle, ClassName(), lpWindowName, dwStyle, rc.left, rc.top,
			rc.right- rc.left, rc.bottom- rc.top, hWndParent, hMenu, GetModuleHandle(NULL), this
			);

		return (m_hWnd ? TRUE : FALSE);
	}
	void Destroy()
	{
		UnregisterClassA(ClassName(), GetModuleHandleA(NULL));
	}
	HWND Window() const { return m_hWnd; }
	void centerWindow(HWND parentWindow)
	{
		RECT parentRect, rect;
		GetWindowRect(parentWindow, &parentRect);
		GetWindowRect(m_hWnd, &rect);
		SetWindowPos(m_hWnd, NULL,	parentRect.left + ((parentRect.right - parentRect.left) / 2) - ((rect.right-rect.left) / 2),
									parentRect.top + ((parentRect.bottom - parentRect.top) / 2) - ((rect.bottom-rect.top) / 2), 
					0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

protected:

	virtual LPCSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	static BOOL CALLBACK SetChildFont(HWND hwndChild, LPARAM lParam)
	{
		HFONT hFont = (HFONT)lParam;
		SendMessage(hwndChild, WM_SETFONT, (WPARAM)hFont, TRUE);
		return TRUE;
	}

	HWND m_hWnd;
};