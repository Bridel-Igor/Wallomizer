#include "CollectionManager.h"

#include <ctime>

#include "App.h"
#include "MainWindow.h"
#include "TrayWindow.h"
#include "UserCollection.h"
#include "LocalCollection.h"
#include "SearchCollection.h"

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

bool CollectionManager::saveSettings(FILE* pFile) const
{
	wchar_t wsPath[MAX_PATH];
	wcscpy_s(wsPath, MAX_PATH, m_app.getWinUtils().getRoamingDir());
	wcscat_s(wsPath, MAX_PATH, L"CollectionManager.dat");
	_wfopen_s(&pFile, wsPath, L"wb");
	if (pFile != NULL)
	{
		fwrite(&CollectionManager::COLLECTION_MANAGER_FILE_VERSION, sizeof(CollectionManager::COLLECTION_MANAGER_FILE_VERSION), 1, pFile);
		const unsigned int size = (unsigned int)m_pCollections.size();
		fwrite(&size, sizeof(size), 1, pFile);
		for (auto pCollection : m_pCollections)
			pCollection->saveSettings(pFile);
		fclose(pFile);
		return true;
	}
	return false;
}

bool CollectionManager::loadSettings(FILE* pFile, unsigned short fileVersion)
{
	Timer::LoadingGuard loading = m_app.getTimer().loadingGuard();
	Player::updateTimer(m_app, true);
	wchar_t wsPath[MAX_PATH];
	wcscpy_s(wsPath, MAX_PATH, m_app.getWinUtils().getRoamingDir());
	wcscat_s(wsPath, MAX_PATH, L"CollectionManager.dat");
	_wfopen_s(&pFile, wsPath, L"rb");
	if (pFile != NULL)
	{
		fread(&fileVersion, sizeof(fileVersion), 1, pFile);
		if (fileVersion >= 2U && fileVersion <= CollectionManager::COLLECTION_MANAGER_FILE_VERSION)
		{
			clear();
			unsigned int nCollections;
			fread(&nCollections, sizeof(nCollections), 1, pFile);
			BaseCollection* pTmpCollection;
			for (unsigned int i = 0; i < nCollections; i++)
			{
				Collection::Type collectionType;
				fread(&collectionType, sizeof(collectionType), 1, pFile);
				if (collectionType == Collection::Type::local)
					pTmpCollection = new LocalCollection(*this);
				else if (collectionType == Collection::Type::user)
					pTmpCollection = new UserCollection(m_app);
				else if (collectionType == Collection::Type::search)
					pTmpCollection = new SearchCollection(m_app);
				else
					break;
				pTmpCollection->loadSettings(pFile, fileVersion);
				pTmpCollection->setValid(true);
				m_pCollections.push_back(pTmpCollection);
			}
			fclose(pFile);
		}
		else
		{
			fclose(pFile);
			saveSettings();
			MessageBox(NULL, "Incompatible collection list file. Collection list was reset.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
		}
	}
	updateNumber();
	if (MainWindow::s_pMainWindow && MainWindow::s_pMainWindow->isReady())
		MainWindow::s_pMainWindow->updateCollectionItems();
	// TODO: is any of these needed?
	//if (m_app.getTimer().getStatus() == Timer::Status::playing)
	//	m_app.getTimer().abortDelay();
	// BUG: this thread runs faster than tray's thread
	// if (m_uiNumber == 0 && TrayWindow::s_pTrayWindow && TrayWindow::s_pTrayWindow->isReady()) 
	//	PostMessageA(TrayWindow::s_pTrayWindow->hWnd(), WM_COMMAND, (WPARAM)TrayWindow::s_pTrayWindow->btnSettings.hMenu(), NULL);
	return true;
}

Wallpaper CollectionManager::getWallpaper(unsigned int _index) const
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
				return Wallpaper::getEmptyWallpaper();
			return m_pCollections[i]->getWallpaper(index + m_pCollections[i]->getNumber());
		}
	}
	return Wallpaper::getEmptyWallpaper();
}

void CollectionManager::openCollectionSettingsWindow(HWND)
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
	Player::updateTimer(m_app, true);
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

void CollectionManager::addCollection(Collection::Type collectionType)
{
	BaseCollection* pCollection = nullptr;
	switch (collectionType)
	{
	case Collection::Type::local:	pCollection = new LocalCollection(*this);	break;
	case Collection::Type::user:	pCollection = new UserCollection(m_app);	break;
	case Collection::Type::search:	pCollection = new SearchCollection(m_app);	break;
	}
	if (pCollection == nullptr)
		return;
	m_pCollections.push_back(pCollection);
	m_pCollections.back()->openCollectionSettingsWindow(MainWindow::s_pMainWindow->hWnd());
	if (!m_pCollections.back()->isValid())
	{
		delete m_pCollections.back();
		m_pCollections.pop_back();
	}
	else
		reloadSettings();
}

void CollectionManager::eraseCollection(int index)
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
	if (m_uiNumber <= 0)
		return Wallpaper(Collection::Type::none, L"");
	const int randomFromAll = m_uniformIntDistribution(m_randomGenerator);
	return getWallpaper(randomFromAll);
}
