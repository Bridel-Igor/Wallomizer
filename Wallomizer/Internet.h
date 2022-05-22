#pragma once

#include <Windows.h>
#include <WinInet.h>

class Internet
{
public:
	Internet();
	Internet(const Internet&) = delete;
	Internet(Internet&&) = delete;
	Internet& operator=(const Internet&) = delete;
	Internet& operator=(Internet&&) = delete;
	~Internet();

	bool DownloadToBuffer(const wchar_t* wsURL, DWORD bufferSize = BUFFER_SIZE_DEFAULT);
	bool DownloadToFile(const wchar_t* wsURL, const wchar_t* wsPath);
	bool parse(const char* sKey, bool bFromLastPos = false);
	bool parse(const char* sKey, wchar_t* wsValue, bool bFromLastPos = false);
	bool parse(const char* sKey, unsigned int& uValue, bool bFromLastPos = false);

private:
	static constexpr DWORD BUFFER_SIZE_DEFAULT = 32768;

	HINTERNET m_hInternetSession = nullptr;
	char *m_pBuffer = nullptr, *m_pCurrent = nullptr;
	DWORD m_currentBufferSize = 0;
};