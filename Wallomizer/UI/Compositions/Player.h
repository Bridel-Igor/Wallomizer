#pragma once

#include <list>

#include "Static.h"
#include "IconButton.h"
#include "CollectionManager.h"

class App;

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
	Player(IComponent* pParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, App& app, DWORD additionalStyles = 0UL);
	~Player();
	bool click(WPARAM& wParam);
	bool draw(LPDRAWITEMSTRUCT& pDIS);
	static void updateTimer(App& app, bool isForced = false);
	static void redrawPlayers();
	
private:
	void updateText();

	IconButton btnPrev, btnOpenExternal, btnStop, btnPlay, btnPause, btnFit, btnNext;
	Static stDelayRemained;
	App& m_app;

	static char sTimer[16];
	static std::list<Player*> pPlayers;
};