#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <filesystem>

class Internet
{
public:
	Internet();
	Internet(const Internet&) = delete;
	Internet(Internet&&) = delete;
	Internet& operator=(const Internet&) = delete;
	Internet& operator=(Internet&&) = delete;
	~Internet();

	bool downloadToBuffer(const std::wstring& URL);
	bool downloadToFile(const std::wstring& URL, const std::filesystem::path& path);
	bool parse(std::string_view key, bool fromLastPosition = false);
	bool parse(std::string_view key, std::wstring& value, bool fromLastPos = false);
	bool parse(std::string_view key, std::size_t& value, bool fromLastPos = false);

private:
	void* m_hInternetSession = nullptr;
	std::string m_buffer;
	std::size_t m_currentPosition = 0;
};
