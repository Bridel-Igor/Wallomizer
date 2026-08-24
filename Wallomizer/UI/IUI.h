#pragma once

/// Provides an interface for requesting UI updates from application logic.
class IUI
{
public:
	virtual ~IUI() = default;

	/// Requests the main window to refresh the collection list asynchronously.
	virtual void requestCollectionListUpdate() const = 0;

	/// Requests all open player windows to refresh their player controls asynchronously.
	virtual void requestPlayerUpdate(bool timerOnly = false) const = 0;
};
