#pragma once

#include <Windows.h>
#include <list>
#include <functional>

/// Interface, that should be derived by any visual WinAPI component directly or by deriving another Interface that derives it.
/// Contains Handle to window. Handle can be used by numerous WinAPI functions.
/// Handle lifetime needs to be controlled by derived component class.
class IComponent
{
public:
	IComponent() = delete;
	IComponent(const IComponent&) = delete;
	IComponent(IComponent&&) = delete;
	IComponent& operator=(const IComponent&) = delete;
	IComponent& operator=(IComponent&&) = delete;
	
	/// Method that traverses all children components recursively applying given operation.
	/// 
	/// @param operation - function that will be applied to every child component.
	void traverseChildren(std::function<void(IComponent* pComponent)> operation);
	/// @return Handle to window.
	HWND hWnd() const { return m_hWnd; }

	/// Pointer to parent component.
	IComponent* m_pParent = nullptr;
	/// List of children components.
	std::list<IComponent*> m_pChildren;

protected:
	/// Protected constructor to prevent allocation of an interface object.
	/// 
	/// @param pParent - pointer to parent component
	IComponent(IComponent* pParent);
	virtual ~IComponent();

	/// Handle to window
	HWND m_hWnd = nullptr;
};