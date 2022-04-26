#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <experimental/filesystem>
#include <string>
#include <shlobj_core.h>

#include "LocalCollection.h"
#include "SetLocalCollectionWindow.h"
#include "Settings.h"
#include "Filesystem.h"

const char* extensions[] = { ".jpg", ".jpeg", ".bmp", ".dib", ".png", ".jfif", ".jpe", ".gif", ".tif", ".tiff", 
							 ".wdp", ".heic", ".heif", ".heics", ".heifs", ".avci", ".avcs", ".avif", ".avifs" };
bool isImage(std::experimental::filesystem::v1::directory_entry path)
{
	for (int i = 0; i < 19; i++)
		if (path.path().extension().compare((std::experimental::filesystem::path)extensions[i]) == 0)
			return true;
	return false;
}

LocalCollection::LocalCollection(CollectionManager* _collectionManager)
{
	m_pCollectionManager = _collectionManager;
	number = 0;
	directoryPath[0] = '\0';
}

bool LocalCollection::saveSettings(FILE* pFile)
{
	if (pFile == NULL)
		return false;
	const CollectionType ct = getCollectionType();
	fwrite(&ct, sizeof(ct), 1, pFile);
	fwrite(&isEnabled, sizeof(isEnabled), 1, pFile);
	fwrite(&directoryPath, sizeof(directoryPath), 1, pFile);
	return true;
}

bool LocalCollection::loadSettings(FILE* pFile)
{
	if (pFile == NULL)
		return false;
	
	fread(&isEnabled, sizeof(isEnabled), 1, pFile);
	fread(&directoryPath, sizeof(directoryPath), 1, pFile);

	std::experimental::filesystem::path p1{ directoryPath };
	number = 0;
	if (isEnabled)
		for (auto& p : std::experimental::filesystem::directory_iterator(p1))
			if (isImage(p))
				number++;
	return true;
}

Wallpaper* LocalCollection::getWallpaperInfo(unsigned int index)
{
	Wallpaper* wallpaper = nullptr;
	if (directoryPath == L"" || number <= 0)
		return wallpaper;
	unsigned int i = 0;
	std::experimental::filesystem::path p1{ directoryPath };
	for (auto& p : std::experimental::filesystem::directory_iterator(p1))
		if (isImage(p))
		{
			if (i == index)
			{
				wallpaper = new Wallpaper(CollectionType::local);
				wcscpy_s(wallpaper->getPathW(), MAX_PATH, p.path().generic_wstring().c_str());
				wallpaper->getPathW()[wcslen(wallpaper->getPathW())] = '\0';
				return wallpaper;
			}
			i++;
		}
	return wallpaper;
}

bool LocalCollection::loadWallpaper(Wallpaper *wallpaper)
{
	char buf[BUFSIZ];
	size_t size;
	FILE* source = nullptr, * dest = nullptr;
	_wfopen_s(&source, wallpaper->getPathW(), L"rb");
	wchar_t path[MAX_PATH];
	Filesystem::getRoamingDir(path);
	wcscat_s(path, MAX_PATH, L"Loaded wallpaper.dat");
	_wfopen_s(&dest, path, L"wb");
	if (source == nullptr || dest == nullptr)
		return false;
	while (size = fread(buf, 1, BUFSIZ, source))
		fwrite(buf, 1, size, dest);
	fclose(source);
	fclose(dest);
	return true;
}

LPCWSTR LocalCollection::collectionName() const
{
	wchar_t* name;
	name = new wchar_t[255]{ 0 };

	wcscat_s(name, 255, L" Local: ");
	wcscat_s(name, 255, directoryPath);

	return name;
}

CategoriesAndPurity LocalCollection::getCAP()
{
	return 0;
}

void LocalCollection::openCollectionSettingsWindow()
{
	SetLocalCollectionWindow::windowThread(this, m_pCollectionManager);
}

void LocalCollection::openWallpaperExternal(Wallpaper* wallpaper)
{
	wchar_t imgPath[MAX_PATH];
	wcscpy_s(imgPath, MAX_PATH, wallpaper->getPathW());

	for (int j = 0; j < wcslen(imgPath); j++)
		if (imgPath[j] == '/')
			imgPath[j] = '\\';

	__unaligned ITEMIDLIST* pidl = ILCreateFromPathW(imgPath);
	if (pidl) {
		SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
		ILFree(pidl);
		return;
	}
}