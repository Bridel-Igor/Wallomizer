#include <ShObjIdl.h>

#include "SetLocalCollectionWindow.h"

SetLocalCollectionWindow::SetLocalCollectionWindow(HWND hCaller, CollectionManager* pCollectionManager, LocalCollection* pCollection) :
	IWindow("Local collection", "Set Local Collection Window Class",WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 400, 120),
	m_hCaller(hCaller),
	m_pCollectionManager(pCollectionManager),
	m_pCurrentLocalCollection(pCollection),
	stPath		(this, "Enter path to directory:",	10,		10,		390,	20),
	edPath		(this, m_pCurrentLocalCollection? m_pCurrentLocalCollection->settings.wsDirectoryPath :L"",
														10,		30,		360,	20),
	btnPath		(this, "..",							370,	30,		20,		20),
	cbRecursive (this,									15,		60,		20,		20, 
				m_pCurrentLocalCollection ? m_pCurrentLocalCollection->settings.bRecursive : false),
	stRecursive (this, "and all subdirectories.",		40,		60,		150,	20),
	btnCancel	(this, "Cancel",						10,		90,		185,	20),
	btnOk		(this, "Ok",							205,	90,		185,	20)
{
	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)resources.mainFont);
	centerWindow(m_hCaller);
	EnableWindow(m_hCaller, FALSE);
	SetForegroundWindow(m_hCaller);
	ShowWindow(hWnd(), SW_SHOWNORMAL);
	SetForegroundWindow(hWnd());
}

SetLocalCollectionWindow::~SetLocalCollectionWindow()
{
	ShowWindow(hWnd(), SW_HIDE);
	EnableWindow(m_hCaller, TRUE);
	SetForegroundWindow(m_hCaller);
}

LRESULT SetLocalCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		if (btnOk.isClicked(wParam))
		{
			if (edPath.isEmpty())
			{
				MessageBoxA(nullptr, "Path can't be empty.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}
			edPath.getTextW(m_pCurrentLocalCollection->settings.wsDirectoryPath, 255);
			m_pCurrentLocalCollection->settings.bRecursive = cbRecursive.isChecked();
			if (m_pCurrentLocalCollection->isValid() == false)
				m_pCurrentLocalCollection->setValid(true);
			else
				m_pCollectionManager->reloadSettings();
			DestroyWindow(hWnd);
			return 0;
		}

		if (btnCancel.isClicked(wParam))
		{
			DestroyWindow(hWnd);
			return 0;
		}

		if (btnPath.isClicked(wParam))
		{
			IFileDialog* pfd;
			LPWSTR g_path;
			if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
			{
				DWORD dwOptions;
				if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
				{
					pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
				}
				if (SUCCEEDED(pfd->Show(NULL)))
				{
					IShellItem* psi;
					if (SUCCEEDED(pfd->GetResult(&psi)))
					{
						if (!SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &g_path)))
						{
							MessageBox(NULL, "GetIDListName() failed", NULL, NULL);
						}
						else
							edPath.setTextW(g_path);
						psi->Release();
					}
				}
				pfd->Release();
			}
			return 0;
		}
		if (cbRecursive.isClicked(wParam))
		{
			cbRecursive.click();
			return 0;
		}
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (cbRecursive.draw(pDIS, resources.mainBkBrush))
			return TRUE;
	}
	return 0;

	default:
		return RESULT_DEFAULT;
	}
}