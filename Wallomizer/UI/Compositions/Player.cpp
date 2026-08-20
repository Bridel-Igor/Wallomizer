#include "Player.h"

#include <cstdint>
#include <algorithm>

#include "resource.h"
#include "IWindow.h"
#include "Timer.h"
#include "WallpaperManager.h"
#include "AppState.h"
#include "GraphicsUtils.h"

std::vector<Player*> Player::s_players;

Player::Icons::Icons()
{
	prev = GraphicsUtils::loadIcon(IDI_PREV);
	prevHover = GraphicsUtils::loadIcon(IDI_PREV_HOVER);
	prevDisabled = GraphicsUtils::makeGrayscaleIcon(prev);

	open = GraphicsUtils::loadIcon(IDI_OPEN_EXTERNAL);
	openHover = GraphicsUtils::loadIcon(IDI_OPEN_EXTERNAL_HOVER);
	openDisabled = GraphicsUtils::makeGrayscaleIcon(open);

	stop = GraphicsUtils::loadIcon(IDI_STOP);
	stopHover = GraphicsUtils::loadIcon(IDI_STOP_HOVER);
	stopToggled = GraphicsUtils::loadIcon(IDI_STOP_TOGGLED);

	play = GraphicsUtils::loadIcon(IDI_PLAY);
	playHover = GraphicsUtils::loadIcon(IDI_PLAY_HOVER);
	playDisabled = GraphicsUtils::makeGrayscaleIcon(play);
	playToggled = GraphicsUtils::loadIcon(IDI_PLAY_TOGGLED);

	pause = GraphicsUtils::loadIcon(IDI_PAUSE);
	pauseHover = GraphicsUtils::loadIcon(IDI_PAUSE_HOVER);
	pauseDisabled = GraphicsUtils::makeGrayscaleIcon(pause);
	pauseToggled = GraphicsUtils::loadIcon(IDI_PAUSE_TOGGLED);

	fit = GraphicsUtils::loadIcon(IDI_FIT);
	fitHover = GraphicsUtils::loadIcon(IDI_FIT_HOVER);
	fitDisabled = GraphicsUtils::makeGrayscaleIcon(fit);

	next = GraphicsUtils::loadIcon(IDI_NEXT);
	nextHover = GraphicsUtils::loadIcon(IDI_NEXT_HOVER);
	nextDisabled = GraphicsUtils::makeGrayscaleIcon(next);
}

Player::Icons::~Icons()
{
	DestroyIcon(prev);	DestroyIcon(prevHover);		DestroyIcon(prevDisabled);
	DestroyIcon(open);	DestroyIcon(openHover);		DestroyIcon(openDisabled);
	DestroyIcon(stop);	DestroyIcon(stopHover);		DestroyIcon(stopToggled);
	DestroyIcon(play);	DestroyIcon(playHover);		DestroyIcon(playDisabled);	DestroyIcon(playToggled);
	DestroyIcon(pause);	DestroyIcon(pauseHover);	DestroyIcon(pauseDisabled);	DestroyIcon(pauseToggled);
	DestroyIcon(fit);	DestroyIcon(fitHover);		DestroyIcon(fitDisabled);
	DestroyIcon(next);	DestroyIcon(nextHover);		DestroyIcon(nextDisabled);
}

Player::Player(IComponent* pParent, int xPlayer, int yPlayer, int xTimer, int yTimer, int widthTimer, int heightTimer, const AppState& appState, Timer& timer, WallpaperManager& wallpaperManager, DWORD additionalStyles) :
	m_appState(appState),
	m_timer(timer),
	m_wallpaperManager(wallpaperManager),
	m_icons(resources.get()),
	btnPrev(pParent,	xPlayer,		yPlayer,	20,	20, m_icons.prev,	m_icons.prevHover,	m_icons.prevDisabled,	nullptr,				"Previous"),
	btnOpen(pParent,	xPlayer + 30,	yPlayer,	20,	20, m_icons.open,	m_icons.openHover,	m_icons.openDisabled,	nullptr,				"Source image"),
	btnStop(pParent,	xPlayer + 60,	yPlayer,	20,	20, m_icons.stop,	m_icons.stopHover,	nullptr,				m_icons.stopToggled,	"Stop"),
	btnPlay(pParent,	xPlayer + 90,	yPlayer,	20,	20, m_icons.play,	m_icons.playHover,	m_icons.playDisabled,	m_icons.playToggled,	"Play"),
	btnPause(pParent,	xPlayer + 120,	yPlayer,	20,	20, m_icons.pause,	m_icons.pauseHover,	m_icons.pauseDisabled,	m_icons.pauseToggled,	"Pause"),
	btnFit(pParent,		xPlayer + 150,	yPlayer,	20,	20, m_icons.fit,	m_icons.fitHover,	m_icons.fitDisabled,	nullptr,				"Fit/fill"),
	btnNext(pParent,	xPlayer + 180,	yPlayer,	20,	20, m_icons.next,	m_icons.nextHover,	m_icons.nextDisabled,	nullptr,				"Next"),
	stRemainingTime(pParent, "", xTimer, yTimer, widthTimer, heightTimer, additionalStyles)
{	
	s_players.push_back(this);
	redrawPlayers();
}

Player::~Player()
{
	auto it = std::find(s_players.begin(), s_players.end(), this);
	if (it != s_players.end())
		s_players.erase(it);
}

bool Player::click(WPARAM wParam)
{
	if (btnPrev.isClicked(wParam))
	{
		m_wallpaperManager.previousWallpaper();
		return true;
	}
	if (btnOpen.isClicked(wParam))
	{
		m_wallpaperManager.openCurrentWallpaperExternally();
		return true;
	}
	if (btnStop.isClicked(wParam))
	{ 
		m_wallpaperManager.stop();
		return true;
	}
	if (btnPlay.isClicked(wParam))
	{
		m_wallpaperManager.play();
		redrawPlayers();
		return true;
	}
	if (btnPause.isClicked(wParam))
	{
		m_wallpaperManager.pause();
		redrawPlayers();
		return true;
	}
	if (btnFit.isClicked(wParam))
	{
		m_wallpaperManager.fit();
		return true;
	}
	if (btnNext.isClicked(wParam))
	{
		m_wallpaperManager.nextWallpaper();
		return true;
	}
	return false;
}

bool Player::draw(LPDRAWITEMSTRUCT drawItem) const
{	
	return btnPrev	.draw(drawItem, m_wallpaperManager.canPrevious())
		|| btnOpen	.draw(drawItem, m_wallpaperManager.canOpen())
		|| btnStop	.draw(drawItem, m_wallpaperManager.canStop(),	m_timer.isStopped())
		|| btnPlay	.draw(drawItem, m_wallpaperManager.canPlay(),	m_timer.isPlaying())
		|| btnPause	.draw(drawItem, m_wallpaperManager.canPause(),	m_timer.isPaused())
		|| btnFit	.draw(drawItem, m_wallpaperManager.canFit())
		|| btnNext	.draw(drawItem, m_wallpaperManager.canNext());
}

void Player::updateTimer()
{
	if (s_players.empty())
		return;

	const AppState& appState = s_players[0]->m_appState;
	const Timer& timer = s_players[0]->m_timer;
	char text[16]{};

	if (appState.isExiting())
		sprintf_s(text, 16, "Exiting...");

	else if (appState.isLoading())
		sprintf_s(text, 16, "Loading...");

	else if (appState.isNoWallpapers())
		sprintf_s(text, 16, "No wallpapers.");

	else if (timer.isStopped())
		sprintf_s(text, 16, "Stopped.");

	else if (appState.isRunning())
	{
		const std::uint32_t remainingSeconds = (timer.getRemainingTime() + 999) / 1000;

		const std::uint16_t hours = static_cast<std::uint16_t>(remainingSeconds / 3600);
		const std::uint8_t minutes = static_cast<std::uint8_t>((remainingSeconds / 60) % 60);
		const std::uint8_t seconds = static_cast<std::uint8_t>(remainingSeconds % 60);

		sprintf_s(text, 16, "%u:%02u:%02u", hours, minutes, seconds);
	}

	for (auto pPlayer : s_players)
	{
		pPlayer->stRemainingTime.setText(text);
		InvalidateRect(pPlayer->stRemainingTime.hWnd(), nullptr, FALSE);
	}
}

void Player::redrawPlayers()
{
	updateTimer();
	for (auto pPlayer : s_players)
	{
		InvalidateRect(pPlayer->btnPrev.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->btnOpen.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->btnStop.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->btnPlay.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->btnPause.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->btnFit.hWnd(), nullptr, FALSE);
		InvalidateRect(pPlayer->btnNext.hWnd(), nullptr, FALSE);
	}
}
