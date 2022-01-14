#pragma once

#include "Static.h"
#include "Button.h"

class Player
{
public:
	Player(HWND hParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, DWORD additionalStyles = 0UL);
	~Player();
	bool click(WPARAM& wParam);
	bool draw(LPDRAWITEMSTRUCT& pDIS);
	static void updateTimer(bool forsed = false);
	static void redrawPlayers();

private:
	void updateText();

	Static* stDelayRemained;
	Button* btnPlay, * btnPause, * btnNext, * btnPrev, * btnOpenExternal;
	static char timer[16];
};