#include <Shobjidl.h>

#include "Player.h"
#include "Delay.h"
#include "resource.h"
#include "IWindow.h"

char Player::sTimer[16];
CollectionManager* Player::s_pCollectionManager = nullptr;
std::list<Player*> Player::pPlayers;
HICON	Player::Resources::hIPlay,			Player::Resources::hIPlayHover,			Player::Resources::hIPlayActive,
		Player::Resources::hIPause,			Player::Resources::hIPauseHover,		Player::Resources::hIPauseActive,
		Player::Resources::hIPrev,			Player::Resources::hIPrevHover,			Player::Resources::hIPrevDisabled,
		Player::Resources::hINext,			Player::Resources::hINextHover,
		Player::Resources::hIOpenExternal,	Player::Resources::hIOpenExternalHover,
		Player::Resources::hIStop,			Player::Resources::hIStopHover,			Player::Resources::hIStopActive,
		Player::Resources::hIFit,			Player::Resources::hIFitHover;
unsigned char Player::Resources::refCount = 0;

Player::Resources::Resources()
{
	if (refCount++) // Loading icons only if this is the first player creating
		return;
	hIPlay =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PLAY),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPlayHover =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PLAY_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPlayActive =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PLAY_ACTIVE),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPause =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PAUSE),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPauseHover =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PAUSE_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPauseActive =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PAUSE_ACTIVE),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPrev =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PREV),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPrevHover =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PREV_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPrevDisabled =		(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PREV_DISABLED),		IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hINext =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_NEXT),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hINextHover =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_NEXT_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIOpenExternal =		(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPEN_EXTERNAL),		IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIOpenExternalHover =	(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPEN_EXTERNAL_HOVER),	IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIStop =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_STOP),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIStopHover =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_STOP_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIStopActive =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_STOP_ACTIVE),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIFit =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_FIT),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIFitHover =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_FIT_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
}

Player::Resources::~Resources()
{
	if (--refCount) // Destroying icons only if this is the last player destroying
		return;
	DestroyIcon(hIPlay);
	DestroyIcon(hIPlayHover);
	DestroyIcon(hIPlayActive);
	DestroyIcon(hIPause);
	DestroyIcon(hIPauseHover);
	DestroyIcon(hIPauseActive);
	DestroyIcon(hIPrev);
	DestroyIcon(hIPrevHover);
	DestroyIcon(hIPrevDisabled);
	DestroyIcon(hINext);
	DestroyIcon(hINextHover);
	DestroyIcon(hIOpenExternal);
	DestroyIcon(hIOpenExternalHover);
	DestroyIcon(hIStop);
	DestroyIcon(hIStopHover);
	DestroyIcon(hIStopActive);
	DestroyIcon(hIFit);
	DestroyIcon(hIFitHover);
}

Player::Player(IComponent* pParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, CollectionManager* pCollectionManager, DWORD additionalStyles) :
	btnPrev(pParent,				xPlayer,		yPlayer,	20,		20, resources.hIPrev,			resources.hIPrevHover,			"Previous"),
	btnOpenExternal(pParent,		xPlayer + 30,	yPlayer,	20,		20, resources.hIOpenExternal,	resources.hIOpenExternalHover,	"Source image"),
	btnStop(pParent,				xPlayer + 60,	yPlayer,	20,		20, resources.hIStop,			resources.hIStopHover,			"Stop"),
	btnPlay(pParent,				xPlayer + 90,	yPlayer,	20,		20, resources.hIPlay,			resources.hIPlayHover,			"Play"),
	btnPause(pParent,				xPlayer + 120,	yPlayer,	20,		20, resources.hIPause,			resources.hIPauseHover,			"Pause"),
	btnFit(pParent,					xPlayer + 150,	yPlayer,	20,		20, resources.hIFit,			resources.hIFitHover,			"Fit/fill"),
	btnNext(pParent,				xPlayer + 180,	yPlayer,	20,		20, resources.hINext,			resources.hINextHover,			"Next"),
	stDelayRemained(pParent, "",	xTimer,			yTimer,		widthTimer,	heightTimer, additionalStyles)
{	
	s_pCollectionManager = pCollectionManager;
	updateTimer(true);
	pPlayers.push_back(this);
}

Player::~Player()
{
	pPlayers.erase(std::find(pPlayers.begin(), pPlayers.end(), this));
}

bool Player::click(WPARAM& wParam)
{
	if (s_pCollectionManager == nullptr)
		return false;
	if (btnOpenExternal.isClicked(wParam))
	{
		s_pCollectionManager->openWallpaperExternal();
		return true;
	}
	if (btnPrev.isClicked(wParam))
	{
		Delay::replayDelay();
		s_pCollectionManager->setPreviousWallpaper();
		return true;
	}
	if (btnStop.isClicked(wParam))
	{ 
		Delay::setSlideshowStatus(Delay::SlideshowStatus::stopped);
		redrawPlayers();
		return 0;
	}
	if (btnPlay.isClicked(wParam))
	{
		Delay::setSlideshowStatus(Delay::SlideshowStatus::playing);
		redrawPlayers();
		return true;
	}
	if (btnPause.isClicked(wParam)) // TODO: save session file on pause. And don't rewrite it on exit
	{
		Delay::setSlideshowStatus(Delay::SlideshowStatus::paused);
		redrawPlayers();
		return true;
	}
	if (btnFit.isClicked(wParam)) // HACK: incompatible with win7 and lower
	{ 
		IDesktopWallpaper* pDesktopWallpaper = NULL;
		if (SUCCEEDED(CoCreateInstance(__uuidof(DesktopWallpaper), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pDesktopWallpaper))))
		{
			DESKTOP_WALLPAPER_POSITION pDWPosition;
			pDesktopWallpaper->GetPosition(&pDWPosition);
			pDesktopWallpaper->SetPosition(pDWPosition == DWPOS_FIT ? DWPOS_FILL : DWPOS_FIT);
		}
		return 0;
	}
	if (btnNext.isClicked(wParam))
	{
		Delay::replayDelay();
		s_pCollectionManager->setNextWallpaper();
		return true;
	}
	return false;
}

bool Player::draw(LPDRAWITEMSTRUCT& pDIS)
{	
	if (pDIS->hwndItem == btnPrev.hWnd())
	{
		if (s_pCollectionManager && !s_pCollectionManager->hasPrevious())
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, IWindow::Resources::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, resources.hIPrevDisabled, 0, 0, 0, NULL, DI_NORMAL);
			return true;
		}
		if (btnPrev.draw(pDIS, IWindow::Resources::mainBkBrush))
			return true;
	}
	if (btnOpenExternal.draw(pDIS, IWindow::Resources::mainBkBrush))
		return true;
	if (pDIS->hwndItem == btnStop.hWnd())
	{
		if (Delay::slideshowStatus == Delay::SlideshowStatus::stopped)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, IWindow::Resources::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, resources.hIStopActive, 0, 0, 0, NULL, DI_NORMAL);
			return true;
		}
		if (btnStop.draw(pDIS, IWindow::Resources::mainBkBrush))
			return true;
	}
	if (pDIS->hwndItem == btnPlay.hWnd())
	{
		if (Delay::slideshowStatus == Delay::SlideshowStatus::playing)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, IWindow::Resources::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, resources.hIPlayActive, 0, 0, 0, NULL, DI_NORMAL);
			return true;
		}
		if (btnPlay.draw(pDIS, IWindow::Resources::mainBkBrush))
			return true;
	}
	if (pDIS->hwndItem == btnPause.hWnd())
	{
		if (Delay::slideshowStatus == Delay::SlideshowStatus::paused)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, IWindow::Resources::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, resources.hIPauseActive, 0, 0, 0, NULL, DI_NORMAL);
			return true;
		}
		if (btnPause.draw(pDIS, IWindow::Resources::mainBkBrush))
			return true;
	}
	if (btnFit.draw(pDIS, IWindow::Resources::mainBkBrush))
		return true;
	if (btnNext.draw(pDIS, IWindow::Resources::mainBkBrush))
		return true;
	return false;
}

void Player::updateTimer(bool isForced)
{
	for (auto pPlayer : pPlayers)
	{
		if (!IsWindowVisible(GetParent(pPlayer->btnPrev.hWnd())))
			continue;
		isForced = true;
		break;
	}
	if (!isForced)
		return;

	if (s_pCollectionManager && s_pCollectionManager->m_isLoading)
		strcpy_s(sTimer, "loading...");
	else
	{
		unsigned long remaining = Delay::getRemainingDelay();
		if (remaining % 1000 != 0)
			remaining += 1000;
		char sSec[3], sMin[3], sHour[4];
		_itoa_s((remaining / 1000) % 60, sSec, 10);
		_itoa_s(((remaining / 1000) / 60) % 60, sMin, 10);
		_itoa_s((remaining / 1000) / 3600, sHour, 10);
		sHour[3] = '\0';
		sMin[2] = '\0';
		sSec[2] = '\0';
		strcpy_s(sTimer, sHour);
		strcat_s(sTimer, " : ");
		if (strlen(sMin) == 1)
			strcat_s(sTimer, "0");
		strcat_s(sTimer, sMin);
		strcat_s(sTimer, " : ");
		if (strlen(sSec) == 1)
			strcat_s(sTimer, "0");
		strcat_s(sTimer, sSec);
	}
	for (auto pPlayer : pPlayers)
	{
		if (!isForced && !IsWindowVisible(GetParent(pPlayer->btnPrev.hWnd())))
			continue;
		pPlayer->updateText();
		InvalidateRect(pPlayer->stDelayRemained.hWnd(), NULL, FALSE);
	}
}

void Player::redrawPlayers()
{
	for (auto pPlayer : pPlayers)
	{
		if (!IsWindowVisible(GetParent(pPlayer->btnPrev.hWnd())))
			continue;
		InvalidateRect(pPlayer->btnPrev.hWnd(), NULL, FALSE);
		InvalidateRect(pPlayer->btnOpenExternal.hWnd(), NULL, FALSE);
		InvalidateRect(pPlayer->btnStop.hWnd(), NULL, FALSE);
		InvalidateRect(pPlayer->btnPlay.hWnd(), NULL, FALSE);
		InvalidateRect(pPlayer->btnPause.hWnd(), NULL, FALSE);
		InvalidateRect(pPlayer->btnFit.hWnd(), NULL, FALSE);
		InvalidateRect(pPlayer->btnNext.hWnd(), NULL, FALSE);
		InvalidateRect(pPlayer->stDelayRemained.hWnd(), NULL, FALSE);
	}
}

void Player::updateText()
{
	SetWindowTextA(stDelayRemained.hWnd(), sTimer);
}