#include "Player.h"
#include "CollectionManager.h"
#include "Delay.h"
#include "WindowStyles.h"
#include "TrayWindow.h"
#include "MainWindow.h"

char Player::timer[16];
CollectionManager* Player::s_pCollectionManager = nullptr;

Player::Player(HWND hParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, CollectionManager* _collectionManager, DWORD additionalStyles)
{
	s_pCollectionManager = _collectionManager;

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
	if (btnOpenExternal->isClicked(wParam))
	{
		s_pCollectionManager->openWallpaperExternal();
		return true;
	}
	if (btnPrev->isClicked(wParam))
	{
		Delay::replayDelay();
		s_pCollectionManager->setPreviousWallpaper();
		return true;
	}
	if (btnPlay->isClicked(wParam))
	{
		Delay::startSlideshow();
		redrawPlayers();
		return true;
	}
	if (btnPause->isClicked(wParam)) // TODO: save session file on pause. And don't rewrite it on exit
	{
		Delay::pauseSlideshow();
		redrawPlayers();
		return true;
	}
	if (btnNext->isClicked(wParam))
	{
		Delay::replayDelay();
		s_pCollectionManager->setNextWallpaper();
		return true;
	}
	return false;
}

bool Player::draw(LPDRAWITEMSTRUCT& pDIS)
{	
	if (pDIS->hwndItem == btnPrev->hWnd())
	{
		if (!s_pCollectionManager->hasPrevious())
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
	if (pDIS->hwndItem == btnPlay->hWnd())
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
	if (pDIS->hwndItem == btnPause->hWnd())
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
	if ((!(MainWindow::mainWindow && MainWindow::isReady() && IsWindowVisible(MainWindow::mainWindow->hWnd()) ||
		(TrayWindow::trayWindow && TrayWindow::trayWindow->isReady() && IsWindowVisible(TrayWindow::trayWindow->hWnd())))) && forsed == false)
		return;
	if (s_pCollectionManager && s_pCollectionManager->m_isLoading)
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
	if (MainWindow::mainWindow && MainWindow::isReady() && (forsed || IsWindowVisible(MainWindow::mainWindow->hWnd())))
	{
		MainWindow::mainWindow->player->updateText();
		InvalidateRect(MainWindow::mainWindow->player->stDelayRemained->hWnd(), NULL, FALSE);
	}
	if (TrayWindow::trayWindow && TrayWindow::trayWindow->isReady() && (forsed || IsWindowVisible(TrayWindow::trayWindow->hWnd())))
	{
		TrayWindow::trayWindow->player->updateText();
		InvalidateRect(TrayWindow::trayWindow->player->stDelayRemained->hWnd(), NULL, FALSE);
	}
}

void Player::redrawPlayers()
{
	if (MainWindow::mainWindow && MainWindow::isReady() && IsWindowVisible(MainWindow::mainWindow->hWnd()))
	{
		MainWindow::mainWindow->player->updateText();
		InvalidateRect(MainWindow::mainWindow->player->btnPrev->hWnd(), NULL, FALSE);
		InvalidateRect(MainWindow::mainWindow->player->btnOpenExternal->hWnd(), NULL, FALSE);
		InvalidateRect(MainWindow::mainWindow->player->btnPlay->hWnd(), NULL, FALSE);
		InvalidateRect(MainWindow::mainWindow->player->btnPause->hWnd(), NULL, FALSE);
		InvalidateRect(MainWindow::mainWindow->player->btnNext->hWnd(), NULL, FALSE);
		InvalidateRect(MainWindow::mainWindow->player->stDelayRemained->hWnd(), NULL, FALSE);
	}
	if (TrayWindow::trayWindow && TrayWindow::trayWindow->isReady() && IsWindowVisible(TrayWindow::trayWindow->hWnd()))
	{
		TrayWindow::trayWindow->player->updateText();
		InvalidateRect(TrayWindow::trayWindow->hWnd(), NULL, FALSE);
	}
}

void Player::updateText()
{
	SetWindowTextA(stDelayRemained->hWnd(), timer);
}

void Player::mouseHovering(WPARAM wParam)
{
	btnPrev->mouseHovering(wParam);
	btnOpenExternal->mouseHovering(wParam);
	btnPlay->mouseHovering(wParam);
	btnPause->mouseHovering(wParam);
	btnNext->mouseHovering(wParam);
}