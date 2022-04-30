#pragma once

#include <list>

#include "Static.h"
#include "IconButton.h"
#include "CollectionManager.h"

class Player
{
public:
	Player(HWND hParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, CollectionManager* pCollectionManager, DWORD additionalStyles = 0UL);
	~Player();
	bool click(WPARAM& wParam);
	bool draw(LPDRAWITEMSTRUCT& pDIS);
	void mouseHovering(WPARAM wParam);
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