#pragma once

#include <cstdint>
#include <string>
#include <filesystem>

class Settings
{
public:
	struct Data
	{
		std::uint32_t prevCount = 5;
		std::uint32_t delay = 300000;
		std::wstring username{};
		std::wstring apiKey{};
		std::uint8_t loadOnStartup = 0;

		bool validate() const;
		bool validateDelay() const;
		bool validateUsername() const;
		bool validateApiKeyLength() const;
		bool validateLoadOnStartup() const;
		
	private:
		static constexpr std::uint32_t MIN_DELAY = 10000;
		static constexpr std::uint32_t MAX_DELAY = 3599999000;
	};

public:
	explicit Settings(std::filesystem::path roamingPath);
	Settings(const Settings&) = delete;
	Settings& operator=(const Settings&) = delete;
	Settings(Settings&&) = delete;
	Settings& operator=(Settings&&) = delete;

	void resetSettings() { m_data = {}; };
	bool saveSettings() const;
	bool loadSettings();
	bool isApiKeyUsed() const noexcept { return !m_data.apiKey.empty(); }

	Data& getData() noexcept { return m_data; }
	const Data& getData() const noexcept { return m_data; }
	
private:
	static constexpr std::uint16_t FILE_VERSION = 3;
	
	Data m_data;
	const std::filesystem::path m_filePath;
};
