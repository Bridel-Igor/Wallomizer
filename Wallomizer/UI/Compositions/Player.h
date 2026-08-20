#pragma once

#include <vector>

#include "IconButton.h"
#include "Static.h"
#include "SharedResources.h"

class AppState;
class Timer;
class WallpaperManager;

class Player
{
private:
	struct Icons
	{
		Icons();
		~Icons();

		HICON	prev = nullptr,		prevHover = nullptr,	prevDisabled = nullptr,
				open = nullptr,		openHover = nullptr,	openDisabled = nullptr,
				stop = nullptr,		stopHover = nullptr,	stopToggled = nullptr,
				play = nullptr,		playHover = nullptr,	playDisabled = nullptr,		playToggled = nullptr,
				pause = nullptr,	pauseHover = nullptr,	pauseDisabled = nullptr,	pauseToggled = nullptr,
				fit = nullptr,		fitHover = nullptr,		fitDisabled = nullptr,
				next = nullptr,		nextHover = nullptr,	nextDisabled = nullptr;
	};

public:
	Player(IComponent* pParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, 
		const AppState& appState, Timer& timer, WallpaperManager& wallpaperManager, DWORD additionalStyles = 0UL);
	~Player();
	bool click(WPARAM wParam);
	bool draw(LPDRAWITEMSTRUCT drawItem) const;
	static void updateTimer();
	static void redrawPlayers();
	
private:
	const AppState& m_appState;
	Timer& m_timer;
	WallpaperManager& m_wallpaperManager;

	SharedResources<Icons> resources;
	const Icons& m_icons;
	IconButton btnPrev, btnOpen, btnStop, btnPlay, btnPause, btnFit, btnNext;
	Static stRemainingTime;

	static std::vector<Player*> s_players;
};
