#include "Player.h"

#include <cstdint>
#include <cstdio>

#include "resource.h"
#include "IWindow.h"
#include "WinUtils.h"
#include "Timer.h"
#include "WallpaperManager.h"

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

Player::Player(IComponent* pParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, const WinUtils& winUtils, Timer& timer, WallpaperManager& wallpaperManager, DWORD additionalStyles) :
	btnPrev(pParent,				xPlayer,		yPlayer,	20,		20, resources.hIPrev,			resources.hIPrevHover,			"Previous"),
	btnOpenExternal(pParent,		xPlayer + 30,	yPlayer,	20,		20, resources.hIOpenExternal,	resources.hIOpenExternalHover,	"Source image"),
	btnStop(pParent,				xPlayer + 60,	yPlayer,	20,		20, resources.hIStop,			resources.hIStopHover,			"Stop"),
	btnPlay(pParent,				xPlayer + 90,	yPlayer,	20,		20, resources.hIPlay,			resources.hIPlayHover,			"Play"),
	btnPause(pParent,				xPlayer + 120,	yPlayer,	20,		20, resources.hIPause,			resources.hIPauseHover,			"Pause"),
	btnFit(pParent,					xPlayer + 150,	yPlayer,	20,		20, resources.hIFit,			resources.hIFitHover,			"Fit/fill"),
	btnNext(pParent,				xPlayer + 180,	yPlayer,	20,		20, resources.hINext,			resources.hINextHover,			"Next"),
	stDelayRemained(pParent, "",	xTimer,			yTimer,		widthTimer,	heightTimer, additionalStyles),
	m_winUtils(winUtils),
	m_timer(timer),
	m_wallpaperManager(wallpaperManager)
{	
	updateTimer(m_timer, true);
	pPlayers.push_back(this);
}

Player::~Player()
{
	pPlayers.erase(std::find(pPlayers.begin(), pPlayers.end(), this));
}

bool Player::click(WPARAM& wParam)
{
	if (btnOpenExternal.isClicked(wParam))
	{
		m_wallpaperManager.openCurrentWallpaperExternally();
		return true;
	}
	if (btnPrev.isClicked(wParam))
	{
		m_timer.repeat();
		m_wallpaperManager.setPreviousWallpaper();
		return true;
	}
	if (btnStop.isClicked(wParam))
	{ 
		m_timer.stop();
		redrawPlayers();
		return 0;
	}
	if (btnPlay.isClicked(wParam))
	{
		m_timer.play();
		redrawPlayers();
		return true;
	}
	if (btnPause.isClicked(wParam))
	{
		m_timer.pause();
		redrawPlayers();
		return true;
	}
	if (btnFit.isClicked(wParam)) 
	{
		m_winUtils.flipWallpaperStyle();
		m_winUtils.updateDesktopBackground(m_timer.getStatus() != Timer::Status::stopped);
		Player::redrawPlayers();
		return true;
	}
	if (btnNext.isClicked(wParam))
	{
		m_timer.repeat();
		m_wallpaperManager.setNextWallpaper();
		return true;
	}
	return false;
}

bool Player::draw(LPDRAWITEMSTRUCT& pDIS)
{	
	if (pDIS->hwndItem == btnPrev.hWnd())
	{
		if (!m_wallpaperManager.hasPrevious())
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
		if (m_timer.getStatus() == Timer::Status::stopped)
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
		if (m_timer.getStatus() == Timer::Status::playing)
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
		if (m_timer.getStatus() == Timer::Status::paused)
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

void Player::updateTimer(Timer& timer, bool isForced)
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

	char text[16] = "loading...";
	if (!timer.isLoading())
	{
		const uint32_t remainingSeconds = (timer.getRemainingTime() + 999) / 1000;

		const uint16_t hours = static_cast<uint16_t>(remainingSeconds / 3600);
		const uint8_t minutes = static_cast<uint8_t>((remainingSeconds / 60) % 60);
		const uint8_t seconds = static_cast<uint8_t>(remainingSeconds % 60);

		sprintf_s(text, 16, "%u:%02u:%02u", hours, minutes, seconds);
	}

	for (auto pPlayer : pPlayers)
	{
		if (!isForced && !IsWindowVisible(GetParent(pPlayer->btnPrev.hWnd())))
			continue;
		pPlayer->updateText(text);
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

void Player::updateText(const char* text)
{
	SetWindowTextA(stDelayRemained.hWnd(), text);
}
