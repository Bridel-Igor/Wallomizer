#include "Player.h"
#include "CollectionManager.h"
#include "Delay.h"
#include "WindowStyles.h"
#include "TrayWindow.h"
#include "MainWindow.h"

char Player::timer[16];

Player::Player(HWND hParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, DWORD additionalStyles)
{
	btnPrev = new IconButton(hParent,			xPlayer,		yPlayer,	20,		20, WindowStyles::hIPrev, WindowStyles::hIPrevHover);
	btnOpenExternal = new IconButton(hParent,	xPlayer + 30,	yPlayer,	20,		20, WindowStyles::hIOpenExternal, WindowStyles::hIOpenExternalHover);
	btnPlay = new IconButton(hParent,			xPlayer + 60,	yPlayer,	20,		20, WindowStyles::hIPlay, WindowStyles::hIPlayHover);
	btnPause = new IconButton(hParent,			xPlayer + 90,	yPlayer,	20,		20, WindowStyles::hIPause, WindowStyles::hIPauseHover);
	btnNext = new IconButton(hParent,			xPlayer + 120,	yPlayer,	20,		20, WindowStyles::hINext, WindowStyles::hINextHover);

	stDelayRemained = new Static(hParent, "",	xTimer,			yTimer,		widthTimer,	heightTimer, additionalStyles);
}

Player::~Player()
{
	delete btnPlay;
	delete btnPause;
	delete btnNext;
	delete btnPrev;
	delete btnOpenExternal;
	delete stDelayRemained;
}

bool Player::click(WPARAM& wParam)
{
	if COMMANDEVENT(btnOpenExternal)
	{
		CollectionManager::openWallpaperExternal();
		return true;
	}
	if COMMANDEVENT(btnPrev)
	{
		Delay::replayDelay();
		CollectionManager::setPreviousWallpaper();
		return true;
	}
	if COMMANDEVENT(btnPlay)
	{
		Delay::startSlideshow();
		redrawPlayers();
		return true;
	}
	if COMMANDEVENT(btnPause) // TODO: save session file on pause. And don't rewrite it on exit
	{
		Delay::pauseSlideshow();
		redrawPlayers();
		return true;
	}
	if COMMANDEVENT(btnNext)
	{
		Delay::replayDelay();
		CollectionManager::setNextWallpaper();
		return true;
	}
	return false;
}

bool Player::draw(LPDRAWITEMSTRUCT& pDIS)
{	
	if (pDIS->hwndItem == btnPrev->hWnd)
	{
		if (!CollectionManager::isPrevious())
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, WindowStyles::hIPrevDisabled, 0, 0, 0, NULL, DI_NORMAL);
			return true;
		}
		if (btnPrev->draw(pDIS, WindowStyles::mainBkBrush))
			return true;
	}
	if (btnOpenExternal->draw(pDIS, WindowStyles::mainBkBrush))
		return true;
	if (pDIS->hwndItem == btnPlay->hWnd)
	{
		if (Delay::bRunSlideshow)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, WindowStyles::hIPlayActive, 0, 0, 0, NULL, DI_NORMAL);
			return true;
		}
		if (btnPlay->draw(pDIS, WindowStyles::mainBkBrush))
			return true;
	}
	if (pDIS->hwndItem == btnPause->hWnd)
	{
		if (!Delay::bRunSlideshow)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, WindowStyles::hIPauseActive, 0, 0, 0, NULL, DI_NORMAL);
			return true;
		}
		if (btnPause->draw(pDIS, WindowStyles::mainBkBrush))
			return true;
	}
	if (btnNext->draw(pDIS, WindowStyles::mainBkBrush))
		return true;
	return false;
}

void Player::updateTimer(bool forsed)
{
	if ((!(MainWindow::mainWindow && IsWindowVisible(MainWindow::mainWindow->Window()) ||
		(TrayWindow::trayWindow && IsWindowVisible(TrayWindow::trayWindow->Window())))) && forsed == false)
		return;
	if (CollectionManager::bLoading)
		strcpy_s(timer, "loading...");
	else
	{
		unsigned long remaining = Delay::getRemainingDelay();
		if (remaining % 1000 != 0)
			remaining += 1000;
		char sec[3], min[3], hour[4];
		_itoa_s((remaining / 1000) % 60, sec, 10);
		_itoa_s(((remaining / 1000) / 60) % 60, min, 10);
		_itoa_s((remaining / 1000) / 3600, hour, 10);
		hour[3] = '\0';
		min[2] = '\0';
		sec[2] = '\0';
		strcpy_s(timer, hour);
		strcat_s(timer, " : ");
		if (strlen(min) == 1)
			strcat_s(timer, "0");
		strcat_s(timer, min);
		strcat_s(timer, " : ");
		if (strlen(sec) == 1)
			strcat_s(timer, "0");
		strcat_s(timer, sec);
	}
	if (MainWindow::mainWindow && (forsed || IsWindowVisible(MainWindow::mainWindow->Window())))
	{
		MainWindow::mainWindow->player->updateText();
		InvalidateRect(MainWindow::mainWindow->player->stDelayRemained->hWnd, NULL, FALSE);
	}
	if (TrayWindow::trayWindow && (forsed || IsWindowVisible(TrayWindow::trayWindow->Window())))
	{
		TrayWindow::trayWindow->player->updateText();
		InvalidateRect(TrayWindow::trayWindow->player->stDelayRemained->hWnd, NULL, FALSE); // BUG: stDelayRemained can be invalid during an app exit
	}
}

void Player::redrawPlayers()
{
	if (MainWindow::mainWindow && IsWindowVisible(MainWindow::mainWindow->Window()))
	{
		MainWindow::mainWindow->player->updateText();
		InvalidateRect(MainWindow::mainWindow->player->btnPrev->hWnd, NULL, FALSE);
		InvalidateRect(MainWindow::mainWindow->player->btnOpenExternal->hWnd, NULL, FALSE);
		InvalidateRect(MainWindow::mainWindow->player->btnPlay->hWnd, NULL, FALSE);
		InvalidateRect(MainWindow::mainWindow->player->btnPause->hWnd, NULL, FALSE);
		InvalidateRect(MainWindow::mainWindow->player->btnNext->hWnd, NULL, FALSE);
		InvalidateRect(MainWindow::mainWindow->player->stDelayRemained->hWnd, NULL, FALSE);
	}
	if (TrayWindow::trayWindow && IsWindowVisible(TrayWindow::trayWindow->Window()))
	{
		TrayWindow::trayWindow->player->updateText();
		InvalidateRect(TrayWindow::trayWindow->Window(), NULL, FALSE);
	}
}

void Player::updateText()
{
	SetWindowTextA(stDelayRemained->hWnd, timer);
}

bool Player::notify(HWND hWnd)
{
	btnPrev->mouseHovering(hWnd == btnPrev->hWnd);
	btnOpenExternal->mouseHovering(hWnd == btnOpenExternal->hWnd);
	btnPlay->mouseHovering(hWnd == btnPlay->hWnd);
	btnPause->mouseHovering(hWnd == btnPause->hWnd);
	btnNext->mouseHovering(hWnd == btnNext->hWnd);
	return true;
}