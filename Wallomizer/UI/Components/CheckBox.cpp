#include "CheckBox.h"

#include <Windows.h>

#include "resource.h"

HICON CheckBox::Resources::hICheckBox, 
	CheckBox::Resources::hICheckBoxChecked, 
	CheckBox::Resources::hICheckBoxHover, 
	CheckBox::Resources::hICheckBoxCheckedHover;
std::uint16_t CheckBox::Resources::refCount = 0;

CheckBox::Resources::Resources()
{
	if (refCount++) // Loading icons only if this is the first CheckBox created
		return;
	hICheckBox =				static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECKBOX),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hICheckBoxChecked =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECKBOX_CHECKED),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hICheckBoxHover =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECKBOX_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hICheckBoxCheckedHover =	static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECKBOX_CHECKED_HOVER),	IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
}

CheckBox::Resources::~Resources()
{
	if (--refCount) // Destroying icons only if this is the last CheckBox destroyed
		return;
	DestroyIcon(hICheckBoxCheckedHover);
	DestroyIcon(hICheckBoxHover);
	DestroyIcon(hICheckBoxChecked);
	DestroyIcon(hICheckBox);
}

CheckBox::CheckBox(IComponent* pParent, int x, int y, int width, int height, bool isChecked, DWORD additionalStyles, DWORD additionalExStyles) :
	IHoverable(pParent),
	m_checked(isChecked)
{
	m_hWnd = CreateWindowExA(additionalExStyles, "Button", "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, parent()->hWnd(), hMenu(), nullptr, nullptr);
}

CheckBox::~CheckBox()
{
	DestroyWindow(m_hWnd);
}

void CheckBox::setChecked(bool state) noexcept
{
	m_checked = state;
	InvalidateRect(m_hWnd, nullptr, FALSE);
}

bool CheckBox::draw(LPDRAWITEMSTRUCT pDIS, HBRUSH bkgrnd)
{
	if (pDIS->hwndItem != m_hWnd)
		return false;
	FillRect(pDIS->hDC, &pDIS->rcItem, bkgrnd);

	DrawIconEx(pDIS->hDC, (pDIS->rcItem.right - 20) / 2, (pDIS->rcItem.bottom - 20) / 2,
		m_checked ? 
		(m_hovering ? resources.hICheckBoxCheckedHover : resources.hICheckBoxChecked) : 
		(m_hovering ? resources.hICheckBoxHover : resources.hICheckBox)
		, 0, 0, 0, nullptr, DI_NORMAL);
	return true;
}
