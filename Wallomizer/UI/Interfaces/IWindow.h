#pragma once

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>

#include "IComponent.h"
#include "resource.h"

#define RESULT_DEFAULT -1

class IWindow : public IComponent
{
public:
	/// Class manages resources (de)allocation for all CollectionItem resources.
	class Resources
	{
	public:
		Resources();
		~Resources();

		static HFONT mainFont;
		static HFONT titleFont;
		static COLORREF mainFontColor;
		static COLORREF titleFontColor;
		static HBRUSH mainBkBrush;
	private:
		static unsigned char refCount;
	}resources;

public:
	IWindow(LPCSTR sWindowName, LPCSTR sClassName, DWORD dwStyle, DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT, IComponent* pParent = nullptr);
	~IWindow();
	virtual void windowLoop();
	void centerWindow(HWND hParent);
	bool isReady() const { return m_isReady; }

protected:
	virtual LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	static BOOL CALLBACK SetChildFont(HWND hChild, LPARAM lParam);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LPCSTR m_sName;
	bool m_isReady = false;
};