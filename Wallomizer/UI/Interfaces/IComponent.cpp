#include "IComponent.h"

#include "UIColor.h"

IComponent::Resources::Resources() noexcept
{
	mainFont = CreateFontA(15, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_DONTCARE, "Arial");
	titleFont = CreateFontA(15, 0, 0, 0, FW_SEMIBOLD, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_DONTCARE, "Arial");
	mainBkBrush = CreateSolidBrush(UIColor::windowBk);
}

IComponent::Resources::~Resources() noexcept
{
	DeleteObject(mainFont);
	DeleteObject(titleFont);
	DeleteObject(mainBkBrush);
}

const IComponent::Resources& IComponent::resources() noexcept
{
	static const Resources resources;
	return resources;
}

void IComponent::setFont(HFONT hFont) noexcept
{
	if (hFont)
		SendMessage(m_hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
}

void IComponent::setText(const std::string& text) noexcept
{
	SetWindowTextA(m_hWnd, text.c_str());
}

void IComponent::setText(const std::wstring& text) noexcept
{
	SetWindowTextW(m_hWnd, text.c_str());
}

std::string IComponent::textA() const
{
	const int length = GetWindowTextLengthA(m_hWnd);

	std::string text(length, '\0');
	GetWindowTextA(m_hWnd, text.data(), length + 1);

	return text;
}

std::wstring IComponent::textW() const
{
	const int length = GetWindowTextLengthW(m_hWnd);

	std::wstring text(length, L'\0');
	GetWindowTextW(m_hWnd, text.data(), length + 1);

	return text;
}
