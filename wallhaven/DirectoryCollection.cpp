#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <string>
#include <Windows.h>
#include "DirectoryCollection.h"
#include "SetDirectoryCollectionWindow.h"

const char* extensions[] = { ".jpg", ".jpeg", ".bmp", ".png" };
bool isImage(std::experimental::filesystem::v1::directory_entry path)
{
	for (int i = 0; i < 4; i++)
		if (path.path().extension().compare((std::experimental::filesystem::path)extensions[i]) == 0)
			return true;
	return false;
}

DirectoryCollection::DirectoryCollection()
{
	number = 0;
	directoryPath[0] = '\0';
}

bool DirectoryCollection::saveSettings(FILE* pFile)
{
	if (pFile != NULL)
	{
		fputs(collectionType(), pFile);
		fputs("\n", pFile);
		fputs(directoryPath, pFile);
		fputs("\n", pFile);
		return true;
	}
	return false;
}

bool DirectoryCollection::loadSettings(FILE* pFile)
{
	if (pFile != NULL)
	{
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

bool DirectoryCollection::setWallpaper(unsigned int index)
{
	if (directoryPath == "")
		return false;
	if (number <= 0)
		return false;
	unsigned int i = 0;
	std::experimental::filesystem::path p1{ directoryPath };
	for (auto& p : std::experimental::filesystem::directory_iterator(p1))
		if (isImage(p))
		{
			if (i == index)
			{
				char imgPath[255];
				strcpy_s(imgPath, p.path().generic_string().c_str());
				if (SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, imgPath, SPIF_UPDATEINIFILE))
					return true;
				else
					return false;
			}
			i++;
		}
	return false;
}

LPCSTR DirectoryCollection::collectionName() const
{
	return directoryPath;
}

void DirectoryCollection::openCollectionSettingsWindow()
{
	SetDirectoryCollectionWindow::windowThread(this);
}