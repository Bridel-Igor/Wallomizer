#include "Internet.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <Windows.h>
#include <WinInet.h>

#pragma comment(lib, "Wininet.lib")

Internet::Internet()
{
	m_hInternetSession = InternetOpenA("Wallomizer", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
}

Internet::~Internet()
{
	InternetCloseHandle(m_hInternetSession);
}

bool Internet::downloadToBuffer(const std::wstring& url, std::size_t bufferSize)
{
	if (url.empty() || bufferSize == 0)
		return false;

	HINTERNET hURL = InternetOpenUrlW(m_hInternetSession, url.c_str(), nullptr, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);

	if (hURL == nullptr)
		return false;

	m_currentPosition = 0;
	m_buffer.assign(bufferSize, '\0');
	
	constexpr std::size_t READ_BUFFER_SIZE = 1024;

	while (m_currentPosition < m_buffer.size())
	{
		const std::size_t bytesAvailable = m_buffer.size() - m_currentPosition;
		const DWORD bytesToRead = static_cast<DWORD>(std::min(bytesAvailable, READ_BUFFER_SIZE));
		DWORD bytesRead = 0;

		if (!InternetReadFile(hURL, m_buffer.data() + m_currentPosition, bytesToRead, &bytesRead))
		{
			m_currentPosition = 0;
			InternetCloseHandle(hURL);
			return false;
		}
		if (bytesRead == 0)
			break;

		m_currentPosition += bytesRead;
	}

	m_buffer.resize(m_currentPosition);
	m_currentPosition = 0;
	InternetCloseHandle(hURL);
	return true;
}

bool Internet::downloadToFile(const std::wstring& url, const std::filesystem::path& path)
{
	if (url.empty() || path.empty())
		return false;

	HINTERNET hURL = InternetOpenUrlW(m_hInternetSession, url.c_str(), nullptr, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
	if (hURL == nullptr)
		return false;

	HANDLE hFile = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		InternetCloseHandle(hURL);
		return false;
	}
	
	auto cleanup = [&]()
	{
		CloseHandle(hFile);
		InternetCloseHandle(hURL);
		std::filesystem::remove(path);
	};

	std::array<char, 1024> buffer{};

	while (true)
	{
		DWORD bytesRead = 0;
		if (!InternetReadFile(hURL, buffer.data(), static_cast<DWORD>(buffer.size()), &bytesRead))
		{
			cleanup();
			return false;
		}
		if (bytesRead == 0)
			break;

		DWORD bytesWritten = 0;
		if (!WriteFile(hFile, buffer.data(), bytesRead, &bytesWritten, nullptr) || bytesWritten != bytesRead)
		{
			cleanup();
			return false;
		}
	}

	CloseHandle(hFile);
	InternetCloseHandle(hURL);
	return true;
}

bool Internet::parse(std::string_view key, bool fromLastPosition)
{
	if (m_buffer.empty() || key.empty())
		return false;

	if (!fromLastPosition)
		m_currentPosition = 0;

	std::string fullKey = "\"";
	fullKey += key;
	fullKey += "\":";

	const std::size_t position = m_buffer.find(fullKey, m_currentPosition);
	if (position == std::string::npos)
		return false;

	m_currentPosition = position + fullKey.size();
	return true;
}

bool Internet::parse(std::string_view key, std::wstring& value, bool fromLastPos)
{
	value.clear();
	if (!parse(key, fromLastPos)
		|| m_currentPosition >= m_buffer.size()
		|| m_buffer[m_currentPosition] != '"')
		return false;

	m_currentPosition++;
	while (m_currentPosition < m_buffer.size())
	{
		const char c = m_buffer[m_currentPosition];
		if (c == '"') // end of parse
		{
			m_currentPosition++;
			return true;
		}

		if (c != '\\') // usual char symbol without '\' prefix
		{
			value += static_cast<wchar_t>(static_cast<unsigned char>(c));
			m_currentPosition++;
			continue;
		}

		m_currentPosition++; // skip '\' prefix
		if (m_currentPosition >= m_buffer.size())
			return false;

		if (m_buffer[m_currentPosition] != 'u') // non unicode symbol
		{
			value += static_cast<wchar_t>(static_cast<unsigned char>(m_buffer[m_currentPosition]));
			m_currentPosition++;
			continue;
		}

		// unicode symbol \uXXXX
		m_currentPosition++;
		if (m_currentPosition + 4 > m_buffer.size())
			return false;

		std::uint32_t code = 0;
		const auto [ptr, error] = std::from_chars(
			m_buffer.data() + m_currentPosition,
			m_buffer.data() + m_currentPosition + 4,
			code, 16);
		if (error != std::errc{} || ptr != m_buffer.data() + m_currentPosition + 4)
			return false;

		value += static_cast<wchar_t>(code);
		m_currentPosition += 4;
		continue;
	}
	return false;
}

bool Internet::parse(std::string_view key, std::size_t& value, bool fromLastPos)
{
	if (!parse(key, fromLastPos))
		return false;
	
	const std::size_t end = m_buffer.find_first_of(",}", m_currentPosition);
	if (end == std::string::npos)
		return false;

	const char* begin = m_buffer.data() + m_currentPosition;
	const char* finish = m_buffer.data() + end;

	value = 0;
	const auto [ptr, error] = std::from_chars(begin, finish, value);
	if (error != std::errc{} || ptr != finish)
		return false;

	m_currentPosition = end;
	return true;
}
