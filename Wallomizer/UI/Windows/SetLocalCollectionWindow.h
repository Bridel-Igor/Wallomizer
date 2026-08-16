#pragma once

#include "IWindow.h"
#include "Static.h"
#include "Edit.h"
#include "CheckBox.h"
#include "Button.h"

class LocalCollection;
class WinUtils;

class SetLocalCollectionWindow : public IWindow
{
public:
	SetLocalCollectionWindow(IWindow* pOwner, const WinUtils& winUtils, LocalCollection& localCollection);

	bool isOk() const noexcept { return m_isOk; }

private:
	LRESULT HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	LocalCollection& m_localCollection;
	const WinUtils& m_winUtils;

	Static stPath, stRecursive;
	Edit edPath;
	CheckBox cbRecursive;
	Button btnOk, btnCancel, btnPath;

	bool m_isOk = false;
};
