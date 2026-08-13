#include "IComponent.h"

#include <algorithm>
#include <stack>

IComponent::IComponent(IComponent* pParent) :
	m_pParent(pParent)
{
	if (m_pParent)
		m_pParent->m_children.push_back(this);
}

IComponent::~IComponent()
{
	if (m_pParent)
	{
		auto it = std::find(m_pParent->m_children.begin(), m_pParent->m_children.end(), this);
		m_pParent->m_children.erase(it);
	}
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

void IComponent::traverseChildren(const std::function<void(IComponent* pComponent)>& operation)
{
	std::stack<IComponent*> depth;
	for (IComponent* child : m_children)
		depth.push(child);
	while (!depth.empty())
	{
		IComponent* component = depth.top();
		depth.pop();

		operation(component);

		for (IComponent* child : component->m_children)
			depth.push(child);
	}
}
