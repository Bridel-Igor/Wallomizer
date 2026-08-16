#include "TrayIcon.h"

#include <stdexcept>
#include <Shellapi.h>
#include <VersionHelpers.h>

#include "UI.h"
#include "resource.h"

TrayIcon::TrayIcon(UI& ui) :
	IWindow(nullptr, "Wallomizer", "TrayIconClass", 0, 0, 0, 0, 0, 0),
	m_ui(ui)
{
	m_hIcon = LoadIcon(GetModuleHandleA(nullptr), MAKEINTRESOURCE(IDI_APP));

	if (!m_hIcon)
		throw std::runtime_error("Failed to load tray icon");

	if (!TrayMessage(NIM_ADD))
	{
		DestroyIcon(m_hIcon);
		m_hIcon = nullptr;
		throw std::runtime_error("Failed to add tray icon");
	}
}

TrayIcon::~TrayIcon()
{
	TrayMessage(NIM_DELETE);
	DestroyIcon(m_hIcon);
}

LRESULT TrayIcon::HandleMessage(HWND, UINT uMsg, WPARAM, LPARAM lParam)
{
	if (uMsg == WM_NOTIFYICONMSG &&	(lParam == WM_RBUTTONUP || lParam == WM_LBUTTONUP))
	{
		m_ui.openTrayWindowAsync(getPosition());
		return 0;
	}

	if (uMsg == WM_QUERYENDSESSION)
	{
		m_ui.queryEndSession();
		return TRUE;
	}

	return RESULT_DEFAULT;
}

BOOL TrayIcon::TrayMessage(DWORD dwMessage) const
{
	NOTIFYICONDATA nid{};
	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd();
	nid.uID = id;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = WM_NOTIFYICONMSG;
	nid.hIcon = m_hIcon;

	lstrcpyA(nid.szTip, "Wallomizer");

	return Shell_NotifyIconA(dwMessage, &nid);
}

POINT TrayIcon::getPosition() const
{
	POINT pt{};
	
	if (IsWindows7OrGreater())
	{
		RECT rect;
		NOTIFYICONIDENTIFIER nii{};
		nii.cbSize = sizeof(NOTIFYICONIDENTIFIER);
		nii.hWnd = hWnd();
		nii.uID = id;
		if (SUCCEEDED(Shell_NotifyIconGetRect(&nii, &rect)))
		{
			pt.x = (rect.left + rect.right) / 2;
			pt.y = (rect.top + rect.bottom) / 2;
			return pt;
		}
	}

	GetCursorPos(&pt);
	return pt;
}
