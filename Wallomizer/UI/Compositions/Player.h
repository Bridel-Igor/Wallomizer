#pragma once

#include <list>

#include "Static.h"
#include "IconButton.h"
#include "CollectionManager.h"

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
						hIOpenExternal, hIOpenExternalHover;
	private:
		static unsigned char refCount;
	}resources;

public:
	Player(IComponent* pParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, CollectionManager* pCollectionManager, DWORD additionalStyles = 0UL);
	~Player();
	bool click(WPARAM& wParam);
	bool draw(LPDRAWITEMSTRUCT& pDIS);
	static void updateTimer(bool isForced = false);
	static void redrawPlayers();
	
private:
	void updateText();

	IconButton btnPrev, btnOpenExternal, btnPlay, btnPause, btnNext;
	Static stDelayRemained;

	static char sTimer[16];
	static CollectionManager* s_pCollectionManager;
	static std::list<Player*> pPlayers;
};