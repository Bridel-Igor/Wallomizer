#pragma once

#include <Windows.h>
#include <functional>
#include <vector>
#include <string>

/// Base interface for visual WinAPI components.
/// Provides a window handle and parent-child component hierarchy.
class IComponent
{
public:
	IComponent() = delete;
	IComponent(const IComponent&) = delete;
	IComponent(IComponent&&) = delete;
	IComponent& operator=(const IComponent&) = delete;
	IComponent& operator=(IComponent&&) = delete;
	
	/// Traverses all child components in depth-first order and applies the given operation.
	/// 
	/// @param operation - Function applied to every child component.
	void traverseChildren(const std::function<void(IComponent* pComponent)>& operation);

	/// Returns the WinAPI window handle.
	HWND hWnd() const noexcept { return m_hWnd; }

	/// Sets the font used to draw the component's text.
	/// The component does not take ownership of the font.
	/// The caller must ensure that the font remains valid while it is used by the component.
	///
	/// @param hFont - handle to the font. Ignored if nullptr.
	void setFont(HFONT hFont) noexcept;

	void setText(const std::string& text) noexcept;
	void setText(const std::wstring& text) noexcept;

	std::string textA() const;
	std::wstring textW() const;

protected:
	/// Protected constructor to prevent direct instantiation.
	/// 
	/// @param pParent - pointer to parent component
	explicit IComponent(IComponent* pParent);

	virtual ~IComponent();

	/// WinAPI window handle.
	HWND m_hWnd = nullptr;

	/// Pointer to parent component.
	IComponent* m_pParent = nullptr;

private:
	/// Child components.
	std::vector<IComponent*> m_children;
};
