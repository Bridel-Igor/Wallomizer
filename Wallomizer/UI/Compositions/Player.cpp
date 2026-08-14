#include "Player.h"

#include <algorithm>

#include "resource.h"
#include "IWindow.h"
#include "WinUtils.h"
#include "Timer.h"
#include "WallpaperManager.h"

std::vector<Player*> Player::pPlayers;
HICON	Player::Resources::hIPlay,			Player::Resources::hIPlayHover,			Player::Resources::hIPlayActive,
		Player::Resources::hIPause,			Player::Resources::hIPauseHover,		Player::Resources::hIPauseActive,
		Player::Resources::hIPrev,			Player::Resources::hIPrevHover,			Player::Resources::hIPrevDisabled,
		Player::Resources::hINext,			Player::Resources::hINextHover,
		Player::Resources::hIOpenExternal,	Player::Resources::hIOpenExternalHover,
		Player::Resources::hIStop,			Player::Resources::hIStopHover,			Player::Resources::hIStopActive,
		Player::Resources::hIFit,			Player::Resources::hIFitHover;
std::uint8_t Player::Resources::refCount = 0;

Player::Resources::Resources()
{
	if (refCount++) // Loading icons only if this is the first player creating
		return;
	hIPlay =				static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PLAY),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIPlayHover =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PLAY_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIPlayActive =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PLAY_ACTIVE),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIPause =				static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PAUSE),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIPauseHover =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PAUSE_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIPauseActive =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PAUSE_ACTIVE),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIPrev =				static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PREV),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIPrevHover =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PREV_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIPrevDisabled =		static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PREV_DISABLED),		IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hINext =				static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_NEXT),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hINextHover =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_NEXT_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIOpenExternal =		static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPEN_EXTERNAL),		IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIOpenExternalHover =	static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPEN_EXTERNAL_HOVER),	IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIStop =				static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_STOP),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIStopHover =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_STOP_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIStopActive =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_STOP_ACTIVE),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIFit =					static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_FIT),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIFitHover =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_FIT_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
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
	m_winUtils(winUtils),
	m_timer(timer),
	m_wallpaperManager(wallpaperManager),
	btnPrev(pParent,			xPlayer,		yPlayer,	20,		20, resources.hIPrev,			resources.hIPrevHover,			"Previous"),
	btnOpenExternal(pParent,	xPlayer + 30,	yPlayer,	20,		20, resources.hIOpenExternal,	resources.hIOpenExternalHover,	"Source image"),
	btnStop(pParent,			xPlayer + 60,	yPlayer,	20,		20, resources.hIStop,			resources.hIStopHover,			"Stop"),
	btnPlay(pParent,			xPlayer + 90,	yPlayer,	20,		20, resources.hIPlay,			resources.hIPlayHover,			"Play"),
	btnPause(pParent,			xPlayer + 120,	yPlayer,	20,		20, resources.hIPause,			resources.hIPauseHover,			"Pause"),
	btnFit(pParent,				xPlayer + 150,	yPlayer,	20,		20, resources.hIFit,			resources.hIFitHover,			"Fit/fill"),
	btnNext(pParent,			xPlayer + 180,	yPlayer,	20,		20, resources.hINext,			resources.hINextHover,			"Next"),
	stDelayRemained(pParent, "",			xTimer,			yTimer,		widthTimer,	heightTimer, additionalStyles)
{	
	updateTimer(m_timer, true);
	pPlayers.push_back(this);
}

Player::~Player()
{
	auto it = std::find(pPlayers.begin(), pPlayers.end(), this);
	if (it != pPlayers.end())
		pPlayers.erase(it);
}

bool Player::click(WPARAM wParam)
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

bool Player::draw(LPDRAWITEMSTRUCT pDIS) const
{	
	if (pDIS->hwndItem == btnPrev.hWnd())
	{
		if (!m_wallpaperManager.hasPrevious())
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, IWindow::Resources::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, resources.hIPrevDisabled, 0, 0, 0, nullptr, DI_NORMAL);
			return true;
		}
		return btnPrev.draw(pDIS, IWindow::Resources::mainBkBrush);
	}
	if (pDIS->hwndItem == btnOpenExternal.hWnd())
	{
		// TODO: gray out if there is no wallpaper in manager
		return btnOpenExternal.draw(pDIS, IWindow::Resources::mainBkBrush);
	}
	if (pDIS->hwndItem == btnStop.hWnd())
	{
		if (m_timer.getStatus() == Timer::Status::stopped)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, IWindow::Resources::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, resources.hIStopActive, 0, 0, 0, nullptr, DI_NORMAL);
			return true;
		}
		return btnStop.draw(pDIS, IWindow::Resources::mainBkBrush);
	}
	if (pDIS->hwndItem == btnPlay.hWnd())
	{
		if (m_timer.getStatus() == Timer::Status::playing)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, IWindow::Resources::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, resources.hIPlayActive, 0, 0, 0, nullptr, DI_NORMAL);
			return true;
		}
		return btnPlay.draw(pDIS, IWindow::Resources::mainBkBrush);
	}
	if (pDIS->hwndItem == btnPause.hWnd())
	{
		if (m_timer.getStatus() == Timer::Status::paused)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, IWindow::Resources::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, resources.hIPauseActive, 0, 0, 0, nullptr, DI_NORMAL);
			return true;
		}
		return btnPause.draw(pDIS, IWindow::Resources::mainBkBrush);
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
		pPlayer->stDelayRemained.setText(text);
		InvalidateRect(pPlayer->stDelayRemained.hWnd(), nullptr, FALSE);
	}
}

void Player::redrawPlayers() noexcept
{
	for (auto pPlayer : pPlayers)
	{
		if (!IsWindowVisible(GetParent(pPlayer->btnPrev.hWnd())))
			continue;
		InvalidateRect(pPlayer->btnPrev.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->btnOpenExternal.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->btnStop.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->btnPlay.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->btnPause.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->btnFit.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->btnNext.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->stDelayRemained.hWnd(), nullptr, FALSE);
	}
}
