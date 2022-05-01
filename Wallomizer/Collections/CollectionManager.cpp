#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <experimental/filesystem>
#include <ctime>

#include "CollectionManager.h"
#include "Settings.h"
#include "MainWindow.h"
#include "TrayWindow.h"
#include "UserCollection.h"
#include "LocalCollection.h"
#include "SearchCollection.h"
#include "Filesystem.h"
#include "Delay.h"

CollectionManager::CollectionManager()
{
	m_randomGenerator = std::mt19937(static_cast<unsigned int>(time(0)));
	m_uniformIntDistribution = std::uniform_int_distribution<int>(0, 0);
	loadSettings();
}

CollectionManager::~CollectionManager()
{
	clear();
}

bool CollectionManager::saveSettings(FILE* pFile) const
{
	wchar_t wsPath[MAX_PATH];
	Filesystem::getRoamingDir(wsPath);
	wcscat_s(wsPath, MAX_PATH, L"CollectionManager.dat");
	_wfopen_s(&pFile, wsPath, L"wb");
	if (pFile != NULL)
	{
		const size_t size = m_pCollections.size();
		fwrite(&size, sizeof(size_t), 1, pFile);
		for (auto pCollection : m_pCollections)
			pCollection->saveSettings(pFile);
		fclose(pFile);
		return true;
	}
	return false;
}

bool CollectionManager::loadSettings(FILE* pFile)
{
	m_isLoading = true;
	Player::updateTimer(true);
	wchar_t wsPath[MAX_PATH];
	Filesystem::getRoamingDir(wsPath);
	wcscat_s(wsPath, MAX_PATH, L"CollectionManager.dat");
	m_isReady = false;
	_wfopen_s(&pFile, wsPath, L"rb");
	if (pFile != NULL)
	{
		clear();
		size_t nCollections;
		fread(&nCollections, sizeof(nCollections), 1, pFile);
		BaseCollection* pTmpCollection;
		for (unsigned int i = 0; i < nCollections; i++)
		{
			CollectionType collectionType;
			fread(&collectionType, sizeof(collectionType), 1, pFile);
			if (collectionType == CollectionType::local)
				pTmpCollection = new LocalCollection(this);
			else if (collectionType == CollectionType::user)
				pTmpCollection = new UserCollection(this);
			else if (collectionType == CollectionType::search)
				pTmpCollection = new SearchCollection(this);
			else
				break;
			pTmpCollection->loadSettings(pFile);
			pTmpCollection->setValid(true);
			m_pCollections.push_back(pTmpCollection);
		}
		fclose(pFile);
	}
	updateNumber();
	if (MainWindow::s_pMainWindow && MainWindow::s_pMainWindow->isReady())
		MainWindow::s_pMainWindow->collectionItemsFrame.updateCollectionItems();
	m_isReady = true;
	if (Delay::bRunSlideshow)
		Delay::abortDelay();
	if (m_uiNumber == 0 && TrayWindow::s_pTrayWindow && TrayWindow::s_pTrayWindow->isReady())
		PostMessageA(TrayWindow::s_pTrayWindow->hWnd(), WM_COMMAND, (WPARAM)TrayWindow::s_pTrayWindow->btnSettings.hMenu(), NULL);
	m_isLoading = false;
	Player::updateTimer(true);
	return true;
}

Wallpaper* CollectionManager::getWallpaperInfo(unsigned int _index) const
{
	int index = _index;
	for (unsigned int i = 0; i < m_pCollections.size(); i++)
	{
		if (!m_pCollections[i]->isEnabled())
			continue;
		index -= m_pCollections[i]->getNumber();
		if (index < 0)
		{
			if (m_pCollections[i] == nullptr || i >= m_pCollections.size() || m_pCollections[i]->getNumber() <= (index + m_pCollections[i]->getNumber()))
				return nullptr;
			return m_pCollections[i]->getWallpaperInfo(index + m_pCollections[i]->getNumber());
		}
	}
	return nullptr;
}

void CollectionManager::reloadSettings()
{
	Delay::beginImageModification();
	saveSettings();
	loadSettings();
	wchar_t wsPath[MAX_PATH];
	Filesystem::getRoamingDir(wsPath);
	wcscat_s(wsPath, MAX_PATH, L"Loaded wallpaper.dat");
	DeleteFileW(wsPath);
	Delay::endImageModification();
	Delay::replayDelay();
}

void CollectionManager::clear()
{
	for (auto pCollection : m_pCollections)
		delete pCollection;
	m_pCollections.clear();
}

void CollectionManager::updateNumber()
{
	m_uiNumber = 0;
	for (unsigned int i = 0; i < m_pCollections.size(); i++)
		m_uiNumber += m_pCollections[i]->isEnabled() ? m_pCollections[i]->getNumber() : 0;
	if (m_uiNumber>0)
		m_uniformIntDistribution = std::uniform_int_distribution<int>(0, m_uiNumber-1);
}

template <typename T> void CollectionManager::addCollection()
{
	BaseCollection* pCollection = new T(this);
	m_pCollections.push_back(pCollection);
	m_pCollections.back()->openCollectionSettingsWindow();
	if (!m_pCollections.back()->isValid())
	{
		delete m_pCollections.back();
		m_pCollections.pop_back();
	}
	else
		reloadSettings();
}
template void CollectionManager::addCollection<LocalCollection>();
template void CollectionManager::addCollection<UserCollection>();
template void CollectionManager::addCollection<SearchCollection>();

void CollectionManager::eraseCollection(int index)
{
	if (m_pCollections[index]!=nullptr)
		delete m_pCollections[index];
	m_pCollections.erase(CollectionManager::m_pCollections.begin() + index);
	saveSettings();
	updateNumber();
	Delay::beginImageModification();
	wchar_t wsPath[MAX_PATH];
	Filesystem::getRoamingDir(wsPath);
	wcscat_s(wsPath, MAX_PATH, L"Loaded wallpaper.dat");
	DeleteFileW(wsPath);
	Delay::endImageModification();
	Delay::abortDelay();
}

void CollectionManager::loadNextWallpaper()
{
	loadRandomWallpaper();
}

void CollectionManager::loadRandomWallpaper()
{
	if (m_uiNumber <= 0)
		return;
	Delay::beginImageModification();
	const int randomFromAll = m_uniformIntDistribution(m_randomGenerator);
	pNext = getWallpaperInfo(randomFromAll);
	loadWallpaper(pNext);
	Delay::endImageModification();
}

void CollectionManager::setLoadedWallpaper(bool setPrevious)
{
	wchar_t wsLoadedPath[MAX_PATH], wsCurrentPath[MAX_PATH];
	Filesystem::getRoamingDir(wsLoadedPath);
	Filesystem::getRoamingDir(wsCurrentPath);
	wcscat_s(wsLoadedPath, MAX_PATH, L"Loaded wallpaper.dat");
	wcscat_s(wsCurrentPath, MAX_PATH, L"Current wallpaper.jpg");

	if (!std::experimental::filesystem::exists(wsLoadedPath))
	{
		Delay::abortDelay();
		return;
	}
	Delay::beginImageModification();
	if (!setPrevious)
	{
		if (pCurrent != nullptr)
		{
			pPreviousList.push_back(pCurrent);
			if (pPreviousList.size() > Settings::prevCount)
			{
				delete pPreviousList.front();
				pPreviousList.pop_front();
			}
		}
		pCurrent = pNext;
	}
	DeleteFileW(wsCurrentPath);
	if (MoveFileW(wsLoadedPath, wsCurrentPath) == 0)
	{
		Delay::endImageModification();
		return;
	}
	wchar_t wsCurrentPathNative[MAX_PATH];
	Filesystem::getRoamingDirNative(wsCurrentPathNative);
	wcscat_s(wsCurrentPathNative, MAX_PATH, L"Current wallpaper.jpg");
	SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, wsCurrentPathNative, SPIF_UPDATEINIFILE);
	Player::redrawPlayers();
	Delay::endImageModification();
}

void CollectionManager::setNextWallpaper()
{
	setLoadedWallpaper();
	loadNextWallpaper();
}

void CollectionManager::setPreviousWallpaper()
{
	if (pPreviousList.empty())
		return;

	loadWallpaper(pPreviousList.back());
	setLoadedWallpaper(true);
	loadNextWallpaper();

	delete pCurrent;
	pCurrent = pPreviousList.back();
	pPreviousList.pop_back();

	Player::redrawPlayers();
}

void CollectionManager::openWallpaperExternal() const
{
	switch (pCurrent->getType())
	{
	case CollectionType::local:
		LocalCollection::openWallpaperExternal(pCurrent);
		break;
	case CollectionType::user:
		UserCollection::openWallpaperExternal(pCurrent);
		break;
	case CollectionType::search:
		SearchCollection::openWallpaperExternal(pCurrent);
		break;
	default:
		return;
	}
}

bool CollectionManager::hasPrevious() const
{
	return !pPreviousList.empty();
}

bool CollectionManager::loadWallpaper(const Wallpaper* pWallpaper)
{
	switch (pWallpaper->getType())
	{
	case CollectionType::local:
		return LocalCollection::loadWallpaper(pWallpaper);
	case CollectionType::user:
		return UserCollection::loadWallpaper(pWallpaper);
	case CollectionType::search:
		return SearchCollection::loadWallpaper(pWallpaper);
	default:
		return false;
	}
}