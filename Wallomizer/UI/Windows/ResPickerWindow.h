#pragma once

#include <string>

#include "IWindow.h"
#include "Static.h"
#include "PushButton.h"
#include "Button.h"
#include "Edit.h"

class ResPickerWindow : public IWindow
{
public:
	ResPickerWindow(IWindow* pOwner, std::wstring& resolution);

private:
	LRESULT HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	static constexpr wchar_t resolutionsTag[] = L"&resolutions=";
	static constexpr wchar_t atleastTag[] = L"&atleast=";

	Static stUltrawide, st169, st1610, st43, st54, stCustom, stX;
	PushButton btnRes[23], btnAtLeast, btnExactly;
	Button btnOk, btnCancel;
	Edit edWidth, edHeight;

	std::wstring& m_resolution;
};
