#pragma once

#include <list>

#include "Static.h"
#include "IconButton.h"

class WinUtils;
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
		static unsigned char refCount;
	}resources;

public:
	Player(IComponent* pParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, const WinUtils& winUtils, Timer& timer, WallpaperManager& wallpaperManager, DWORD additionalStyles = 0UL);
	~Player();
	bool click(WPARAM& wParam);
	bool draw(LPDRAWITEMSTRUCT& pDIS);
	static void updateTimer(Timer& timer, bool isForced = false);
	static void redrawPlayers();
	
private:
	void updateText(const char* text);

	IconButton btnPrev, btnOpenExternal, btnStop, btnPlay, btnPause, btnFit, btnNext;
	Static stDelayRemained;

	const WinUtils& m_winUtils;
	Timer& m_timer;
	WallpaperManager& m_wallpaperManager;

	static std::list<Player*> pPlayers;
};
