#pragma once

#include <string>

#include "IComponent.h"

/// Generic child window component that can contain other components.
class Panel : public IComponent
{
public:
	/// Constructs a Panel component.
	///
	/// @param pParent - pointer to the parent component.
	/// @param className - unique window class name used to register the panel.
	/// @param x, y - coordinates, relative to the parent window.
	/// @param width, height - size of the panel in pixels.
	/// @param bkBrush - brush used to fill the panel background.
	Panel(IComponent* pParent, std::string className, int x, int y, int width, int height, HBRUSH bkBrush);

	~Panel();

private:
	/// Result returned by HandleMessage when the message was not handled.
	static constexpr LRESULT RESULT_NOT_HANDLED = -1;

	/// Window procedure forwarding messages to the corresponding Panel instance.
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// Handles messages received by the panel.
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// Window class name used by the panel.
	std::string m_className;

	/// Brush used to fill the panel background.
	HBRUSH m_bkBrush;
};
