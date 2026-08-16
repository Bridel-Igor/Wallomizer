#include "SetLocalCollectionWindow.h"

#include "LocalCollection.h"
#include "WinUtils.h"

SetLocalCollectionWindow::SetLocalCollectionWindow(IWindow* pOwner, const WinUtils& winUtils, LocalCollection& localCollection) :
	IWindow(pOwner, "Local collection", "Set Local Collection Window Class",WS_CAPTION | WS_SYSMENU, 0, 100, 100, 400, 120),
	m_localCollection(localCollection),
	m_winUtils(winUtils),
	stPath		(this, "Enter path to directory:",			10,		10,		390,	20),
	edPath		(this, m_localCollection.getPath().c_str(),	10,		30,		360,	20),
	btnPath		(this, "..",								370,	30,		20,		20),
	cbRecursive (this,										15,		60,		20,		20, m_localCollection.isRecursive()),
	stRecursive (this, "and all subdirectories.",			40,		60,		150,	20),
	btnCancel	(this, "Cancel",							10,		90,		185,	20),
	btnOk		(this, "Ok",								205,	90,		185,	20)
{}

LRESULT SetLocalCollectionWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			m_localCollection.setPath(edPath.textW());
			m_localCollection.setRecursive(cbRecursive.isChecked());
			m_localCollection.update();
			m_isOk = true;
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnCancel.isClicked(wParam))
		{
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnPath.isClicked(wParam))
		{
			const auto path = m_winUtils.pickDirectory();
			if (!path.empty())
				edPath.setText(path.wstring());
			return 0;
		}
		if (cbRecursive.isClicked(wParam))
		{
			cbRecursive.toggle();
			return 0;
		}
		break;
	}

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT drawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
		if (cbRecursive.draw(drawItem, resources.mainBkBrush))
			return TRUE;
		break;
	}
	}
	
	return RESULT_DEFAULT;
}
