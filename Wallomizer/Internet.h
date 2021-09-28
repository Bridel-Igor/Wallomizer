#pragma once

#include <mutex>
#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")

namespace Internet
{
	extern constexpr DWORD bufferSize = 32768;
	extern char buffer[bufferSize];
	extern std::mutex bufferAccess;

	bool URLDownloadToBuffer(char* URL, char* _buffer = buffer, DWORD _bufferSize = bufferSize);
	bool URLDownloadToFile(char* URL, wchar_t* path);
	char* parse(char* buffer, const char* key, nullptr_t value);
	char* parse(char* buffer, const char* key, char* value);
	char* parse(char* buffer, const char* key, unsigned int* value);
}