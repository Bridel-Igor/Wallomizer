#pragma once

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>

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

		RegisterClass(&wc);

		m_hWnd = CreateWindowExA(
			dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
			);

		return (m_hWnd ? TRUE : FALSE);
	}
	void Destroy()
	{
		UnregisterClassA(ClassName(), GetModuleHandleA(NULL));
	}

	HWND Window() const { return m_hWnd; }

protected:

	virtual LPCSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hWnd;
};

