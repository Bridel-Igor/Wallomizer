#pragma once

#include "IComponent.h"

class Panel : public IComponent
{
public:
	Panel(HWND hParent, LPCSTR className, int x, int y, int width, int height, HBRUSH bkBrush);
	~Panel();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hParent;
	LPCSTR m_sName;
	HBRUSH m_bkBrush;
};