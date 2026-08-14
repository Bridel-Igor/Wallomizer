#pragma once

#include <Windows.h>
#include <string>

/// Base class for visual WinAPI components.
/// Provides a window handle and access to the parent component.
class IComponent
{
public:
	IComponent() = delete;
	IComponent(const IComponent&) = delete;
	IComponent(IComponent&&) = delete;
	IComponent& operator=(const IComponent&) = delete;
	IComponent& operator=(IComponent&&) = delete;
	
	/// @return The WinAPI window handle.
	HWND hWnd() const noexcept { return m_hWnd; }

	/// @return Pointer to the parent component.
	IComponent* parent() const noexcept { return m_pParent; }

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
	explicit IComponent(IComponent* pParent) :
		m_pParent(pParent)
	{}

	virtual ~IComponent() = default;

	/// WinAPI window handle.
	HWND m_hWnd = nullptr;

private:
	/// Pointer to parent component.
	IComponent* m_pParent = nullptr;
};
