#pragma once

#include "IComponent.h"

class Panel : public IComponent
{
public:
	Panel(IComponent* pParent, LPCSTR className, int x, int y, int width, int height, HBRUSH bkBrush);
	~Panel();

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LPCSTR m_sName;
	HBRUSH m_bkBrush;
};