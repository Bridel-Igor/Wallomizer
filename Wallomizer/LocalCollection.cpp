#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <experimental/filesystem>
#include <string>
#include <shlobj_core.h>

#include "LocalCollection.h"
#include "SetLocalCollectionWindow.h"
#include "Settings.h"

const char* extensions[] = { ".jpg", ".jpeg", ".bmp", ".dib", ".png", ".jfif", ".jpe", ".gif", ".tif", ".tiff", 
							 ".wdp", ".heic", ".heif", ".heics", ".heifs", ".avci", ".avcs", ".avif", ".avifs" };
bool isImage(std::experimental::filesystem::v1::directory_entry path)
{
	for (int i = 0; i < 19; i++)
		if (path.path().extension().compare((std::experimental::filesystem::path)extensions[i]) == 0)
			return true;
	return false;
}

LocalCollection::LocalCollection()
{
	number = 0;
	directoryPath[0] = '\0';
}

bool LocalCollection::saveSettings(FILE* pFile)
{
	if (pFile != NULL)
	{
		fputs(collectionType(), pFile);
		fputs("\n", pFile);
		fputs(isEnabled ? "true" : "false", pFile);
		fputs("\n", pFile);
		fputs(directoryPath, pFile);
		fputs("\n", pFile);
		return true;
	}
	return false;
}

bool LocalCollection::loadSettings(FILE* pFile)
{
	if (pFile != NULL)
	{
		char tmpBuffer[10];
		fgets(tmpBuffer, 10, pFile);
		tmpBuffer[strlen(tmpBuffer) - 1] = '\0';
		isEnabled = strcmp(tmpBuffer, "true") == 0 ? true : false;
		fgets(directoryPath, 255, pFile);
		directoryPath[strlen(directoryPath) - 1] = '\0';
		std::experimental::filesystem::path p1{ directoryPath };
		number = 0;
		for (auto& p : std::experimental::filesystem::directory_iterator(p1))
			if (isImage(p))
				number++;
		return true;
	}
	return false;
}

bool LocalCollection::loadWallpaper(unsigned int index)
{
	if (directoryPath == "" || number <= 0)
		return false;
	unsigned int i = 0;
	std::experimental::filesystem::path p1{ directoryPath };
	for (auto& p : std::experimental::filesystem::directory_iterator(p1))
		if (isImage(p))
		{
			if (i == index)
			{
				wchar_t imgPath[255];
				wcscpy_s(imgPath, 255, p.path().generic_wstring().c_str());
				imgPath[wcslen(imgPath)] = '\0';
				char buf[BUFSIZ];
				size_t size;
				FILE *source=nullptr, *dest=nullptr;
				_wfopen_s(&source, imgPath, L"rb");
				_wfopen_s(&dest, L"Resources/Loaded wallpaper.dat", L"wb");
				if (source == nullptr || dest == nullptr)
					return false;
				while (size = fread(buf, 1, BUFSIZ, source))
					fwrite(buf, 1, size, dest);
				fclose(source);
				fclose(dest);
				return true;
			}
			i++;
		}
	return false;
}

LPCSTR LocalCollection::collectionName() const
{
	char* name;
	name = new char[255]{ 0 };

	strcat_s(name, 255, " Local: ");
	strcat_s(name, 255, directoryPath);

	return name;
}

void LocalCollection::openCollectionSettingsWindow()
{
	SetLocalCollectionWindow::windowThread(this);
}

void LocalCollection::openWallpaperExternal(unsigned int index)
{
	if (directoryPath == "" || number <= 0)
		return;
	unsigned int i = 0;
	std::experimental::filesystem::path p1{ directoryPath };
	for (auto& p : std::experimental::filesystem::directory_iterator(p1))
		if (isImage(p))
		{
			if (i == index)
			{
				wchar_t imgPath[255];
				wcscpy_s(imgPath, 255, p.path().generic_wstring().c_str());
				imgPath[wcslen(imgPath)] = '\0';
				for (int j = 0; j < wcslen(imgPath); j++)
					if (imgPath[j] == '/')
						imgPath[j] = '\\';

				ITEMIDLIST* pidl = ILCreateFromPathW(imgPath);
				if (pidl) {
					SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
					ILFree(pidl);
					return;
				}
			}
			i++;
		}
}