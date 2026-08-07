#include "CollectionManager.h"

#include <ctime>

#include "App.h"
#include "MainWindow.h"
#include "TrayWindow.h"
#include "UserCollection.h"
#include "LocalCollection.h"
#include "SearchCollection.h"
#include "BinaryIO.h"

CollectionManager::CollectionManager(App& app) :
	m_app(app)
{
	m_randomGenerator = std::mt19937(static_cast<unsigned int>(time(0)));
	m_uniformIntDistribution = std::uniform_int_distribution<int>(0, 0);
	loadSettings();
}

CollectionManager::~CollectionManager()
{
	clear();
}

bool CollectionManager::saveSettings() const
{
	std::filesystem::path filePath = m_app.getWinUtils().getRoamingDir() / L"CollectionManager.dat";
	const std::uint32_t collectionsCount = static_cast<std::uint32_t>(m_pCollections.size());

	BinaryWriter file(filePath);
	if (!file.isOpen()
		|| !file.write(FILE_VERSION)
		|| !file.write(collectionsCount))
		return false;

	for (auto pCollection : m_pCollections)
		if (!pCollection->saveSettings(file))
			return false;

	return true;
}

bool CollectionManager::loadSettings()
{
	Timer::LoadingGuard loading = m_app.getTimer().loadingGuard();
	Player::updateTimer(m_app.getTimer(), true);

	std::filesystem::path filePath = m_app.getWinUtils().getRoamingDir() / L"CollectionManager.dat";

	BinaryReader file(filePath);
	std::uint16_t fileVersion;
	if (!file.isOpen()
		|| !file.read(fileVersion))
		return false;

	clear();

	switch (fileVersion)
	{
	case 4U:
	case 3U:
	{
		std::uint32_t collectionCount;
		if (!file.read(collectionCount))
			return false;
		BaseCollection* pTmpCollection = nullptr;
		for (std::uint32_t i = 0; i < collectionCount; i++)
		{
			pTmpCollection = nullptr;
			Collection::Type collectionType;
			if (!file.read(collectionType))
				return false;
			switch (collectionType)
			{
			case Collection::Type::local:
				pTmpCollection = new LocalCollection(*this);
				break;
			case Collection::Type::user:
				pTmpCollection = new UserCollection(m_app);
				break;
			case Collection::Type::search:
				pTmpCollection = new SearchCollection(m_app.getSettings(), *this);
				break;
			}
			if (pTmpCollection != nullptr)
			{
				if (pTmpCollection->loadSettings(file, fileVersion))
				{
					pTmpCollection->update();
					m_pCollections.push_back(pTmpCollection);
				}
				else
				{
					delete pTmpCollection;
				}
			}
		}
		break;
	}
	default:
	{
		MessageBox(NULL, "Incompatible collection list file. Collection list was reset.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
		break;
	}
	}

	if (fileVersion != FILE_VERSION)
		saveSettings();

	updateNumber();

	// TODO: is any of these needed?
	// seems like a code to open a main window if there are no collections in list
	//if (m_app.getTimer().getStatus() == Timer::Status::playing)
	//	m_app.getTimer().abortDelay();
	// BUG: this thread runs faster than tray's thread
	// if (m_uiNumber == 0 && TrayWindow::s_pTrayWindow && TrayWindow::s_pTrayWindow->isReady()) 
	//	PostMessageA(TrayWindow::s_pTrayWindow->hWnd(), WM_COMMAND, (WPARAM)TrayWindow::s_pTrayWindow->btnSettings.hMenu(), NULL);
	return true;
}

Wallpaper CollectionManager::getWallpaper(std::uint32_t index) const
{
	std::int64_t current = index;
	for (std::size_t i = 0; i < m_pCollections.size(); i++)
	{
		if (!m_pCollections[i]->isEnabled())
			continue;
		current -= m_pCollections[i]->getNumber();
		if (current < 0)
		{
			if (m_pCollections[i] == nullptr || i >= m_pCollections.size() || m_pCollections[i]->getNumber() <= (current + m_pCollections[i]->getNumber()))
				return Wallpaper::getEmptyWallpaper();
			return m_pCollections[i]->getWallpaper(current + m_pCollections[i]->getNumber());
		}
	}
	return Wallpaper::getEmptyWallpaper();
}

void CollectionManager::openMainWindow()
{ 
	std::thread mainWindowThread([&]()
		{
			try 
			{
				MainWindow mainWindow(m_app);
				mainWindow.windowLoop();
			}
			catch (...)
			{
			}
		});
	mainWindowThread.detach(); // TODO: exception handling. Move thread var to members
}

void CollectionManager::reloadSettings()
{
	saveSettings();
	loadSettings();
	m_app.getWallpaperManager().deleteLoaded();
	m_app.getTimer().repeat();
	Player::updateTimer(m_app.getTimer(), true);
}

void CollectionManager::clear()
{
	for (auto pCollection : m_pCollections)
		delete pCollection;
	m_pCollections.clear();
}

void CollectionManager::updateNumber()
{
	m_number = 0;
	for (std::size_t i = 0; i < m_pCollections.size(); i++)
		m_number += m_pCollections[i]->isEnabled() * static_cast<std::uint32_t>(m_pCollections[i]->getNumber());
	if (m_number>0)
		m_uniformIntDistribution = std::uniform_int_distribution<int>(0, m_number-1);
}

void CollectionManager::addCollection(Collection::Type collectionType)
{
	BaseCollection* pCollection = nullptr;
	switch (collectionType)
	{
	case Collection::Type::local:	pCollection = new LocalCollection(*this);						break;
	case Collection::Type::user:	pCollection = new UserCollection(m_app);						break;
	case Collection::Type::search:	pCollection = new SearchCollection(m_app.getSettings(), *this);	break;
	}
	if (pCollection == nullptr)
		return;
	pCollection->openCollectionSettingsWindow(MainWindow::s_pMainWindow->hWnd());

	m_pCollections.push_back(pCollection);
	reloadSettings();
}

void CollectionManager::eraseCollection(std::size_t index)
{
	if (m_pCollections[index]!=nullptr)
		delete m_pCollections[index];
	m_pCollections.erase(CollectionManager::m_pCollections.begin() + index);
	saveSettings();
	updateNumber();
	m_app.getWallpaperManager().deleteLoaded();
	m_app.getTimer().abort();
}

Wallpaper CollectionManager::getRandomWallpaper()
{
	if (m_number <= 0)
		return Wallpaper(Collection::Type::none, L"");
	const int randomFromAll = m_uniformIntDistribution(m_randomGenerator);
	return getWallpaper(randomFromAll);
}
