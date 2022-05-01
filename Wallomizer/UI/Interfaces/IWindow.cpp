#include <exception>

#include "IWindow.h"

#pragma once

IWindow::IWindow(LPCSTR sWindowName, LPCSTR sClassName, DWORD dwStyle, DWORD dwExStyle,
		int x, int y, int nWidth, int nHeight, HWND hParent) :
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
		rc.right - rc.left, rc.bottom - rc.top, hParent, 0, GetModuleHandle(NULL), this);
	if (m_hWnd == FALSE)
		throw std::exception("Window creation failed.");
}

IWindow::~IWindow()
{
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
		return pThis->HandleMessage(hWnd, uMsg, wParam, lParam);
	else
		return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}