#pragma once

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>

#include "IComponent.h"
#include "WindowStyles.h"
#include "resource.h"

class IWindow : public IComponent
{
public:
	IWindow(LPCSTR sWindowName, LPCSTR sClassName, DWORD dwStyle, DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT);
	~IWindow();
	virtual void windowLoop();
	void centerWindow(HWND hParent);
	bool isReady() const { return m_isReady; }

protected:
	virtual LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	static BOOL CALLBACK SetChildFont(HWND hChild, LPARAM lParam);

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	LPCSTR m_sName;
	bool m_isReady = false;
};