#include "Settings.h"

#include <Windows.h>
#include <string>

#include "BinaryIO.h"

Settings::Settings(std::filesystem::path roamingPath) :
	m_filePath(roamingPath / L"Settings.dat")
{
	loadSettings();
}

bool Settings::saveSettings() const
{
	BinaryWriter file(m_filePath);
	return file.isOpen()
		&& file.write(FILE_VERSION)
		&& file.write(m_data.loadOnStartup)
		&& file.write(m_data.delay)
		&& file.write(m_data.username)
		&& file.write(m_data.apiKey);
}

bool Settings::loadSettings()
{
	if (!std::filesystem::exists(m_filePath))
	{
		saveSettings();
		return true;
	}

	std::uint16_t fileVersion = 0;
	BinaryReader file(m_filePath);
	if (!file.isOpen()
		|| !file.read(fileVersion)
		|| fileVersion != FILE_VERSION
		|| !file.read(m_data.loadOnStartup)
		|| !file.read(m_data.delay)
		|| !file.read(m_data.username)
		|| !file.read(m_data.apiKey)
		|| !m_data.validate())
	{
		resetSettings();
		saveSettings();
		MessageBoxA(nullptr, "Settings file is corrupted or incompatible. Default settings have been restored.", "Wallomizer", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	return true;
}

bool Settings::Data::validate() const
{
	return validateDelay()
		&& validateUsername()
		&& validateApiKeyLength()
		&& validateLoadOnStartup();
}

bool Settings::Data::validateDelay() const
{
	return delay >= MIN_DELAY && delay <= MAX_DELAY;
}

bool Settings::Data::validateUsername() const
{
	return username[63] == L'\0';
}

bool Settings::Data::validateApiKeyLength() const
{
	if (apiKey[32] != L'\0')
		return false;
	const std::size_t length = wcslen(apiKey);
	return length==0 || length==32;
}

bool Settings::Data::validateLoadOnStartup() const
{
	return loadOnStartup == 0 || loadOnStartup == 1;
}
