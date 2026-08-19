#pragma once

#include <cstdint>
#include <vector>

#include "IconButton.h"
#include "Static.h"

class AppState;
class Timer;
class WallpaperManager;

class Player
{
private:
	/// Class manages resources (de)allocation for all player resources.
	class Resources
	{
	public:
		Resources();
		~Resources();
		
		static HICON	hIPlay,			hIPlayHover,		hIPlayActive,
						hIPause,		hIPauseHover,		hIPauseActive,
						hIPrev,			hIPrevHover,		hIPrevDisabled,
						hINext,			hINextHover,
						hIOpenExternal, hIOpenExternalHover,
						hIStop,			hIStopHover,		hIStopActive,
						hIFit,			hIFitHover;
	private:
		static std::uint8_t refCount;
	}resources;

public:
	Player(IComponent* pParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, const AppState& appState, Timer& timer, WallpaperManager& wallpaperManager, DWORD additionalStyles = 0UL);
	~Player();
	bool click(WPARAM wParam);
	bool draw(LPDRAWITEMSTRUCT pDIS) const;
	static void updateTimer();
	static void redrawPlayers();
	
private:
	const AppState& m_appState;
	Timer& m_timer;
	WallpaperManager& m_wallpaperManager;

	IconButton btnPrev, btnOpenExternal, btnStop, btnPlay, btnPause, btnFit, btnNext;
	Static stDelayRemained;

	static std::vector<Player*> pPlayers;
};
